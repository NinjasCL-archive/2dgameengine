#ifndef ECS_H
#define ECS_H

#include <vector>
#include <deque>
#include <unordered_map>
#include <set>
#include <memory>
#include <algorithm>
#include <string>
#include <cstdint>
#include <bitset>
#include <typeindex>
#include "../Pool/Pool.h"

const unsigned int MAX_ENTITIES = 5000;
const unsigned int MAX_COMPONENTS = 32;

///////////////////////////////////////////////////////////////////////////////
// Components
///////////////////////////////////////////////////////////////////////////////
// Components are simple structs (plain data) that hold a numerical Id.
///////////////////////////////////////////////////////////////////////////////
struct IComponent {
	protected:
		static int nextId;
};

// Used to assign a unique id to a component type
template <typename TComponent>
struct Component: public IComponent {
	// Returns the unique id of Component<T>
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};

///////////////////////////////////////////////////////////////////////////////
// Signature
///////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has
// and also helps keep track of which entities a system is interested in.
///////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPONENTS> Signature;

///////////////////////////////////////////////////////////////////////////////
// Entity
///////////////////////////////////////////////////////////////////////////////
// This class is basically just a a wrapper to a simple numerical Id.
// We have some methods to add and remove components for the entity, which are
// just shortcuts to the methods to add/remove from the Registry class.
///////////////////////////////////////////////////////////////////////////////
class Entity {
	private:
		int id;

	public:
		Entity(int id): id(id) {};
		Entity(const Entity& entity) = default;

		int GetId() const;

		// Flags the entity to be killed in the next system update.
		void Kill();

		/* Manage components via requests the parent registry class */
		template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
		template <typename TComponent> void RemoveComponent();
		template <typename TComponent> bool HasComponent() const;
		template <typename TComponent> TComponent& GetComponent() const;

		// Assignment operator and comparison operator
		Entity& operator =(const Entity& other) = default;
		bool operator ==(const Entity& other) const { return id == other.id; }
		bool operator <(const Entity& other) const { return id < other.id; }

		// Holds a pointer to the the parent ECS registry
		class Registry* registry;
};

///////////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////////
// The system processes entities that it's interested in each frame.
// All game systems should inherit from this parent class.
///////////////////////////////////////////////////////////////////////////////
class System {
	private:
		// Which components an entity must have in order for the system to process the entity
		Signature componentSignature;

		// List of all entities that the system is interested in
		std::vector<Entity> entities;

	public:
		System() = default;
		virtual ~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);
		std::vector<Entity> GetSystemEntities() const;
		const Signature& GetComponentSignature() const;

		// Define the component type that the entities must have to be part of the system
		template <typename TComponent> void RequireComponent();
};

///////////////////////////////////////////////////////////////////////////////
// Registry
///////////////////////////////////////////////////////////////////////////////
// The Registry manages the creation and destruction of entities so that
// entities don't show up/disappear mid-frame, and it also contains the main
// methods to create new entities, kill entities, add systems, and components.
///////////////////////////////////////////////////////////////////////////////
class Registry {
	private:
		// Keep track of how many entities were added
		int numEntities = 0;
		
		// List of free entity Ids that were previously removed
		std::deque<int> freeIds;

		// Vector of component pools, each pool contains all the data for a certain component type (vector index = component id, pool index = entity id)
		std::vector<std::shared_ptr<IPool>> componentPools;

		// Vector of component signatures (vector index = entity id), the signature lets us know which components are turned "on" for a specific entity
		std::vector<Signature> entityComponentSignatures;

		// Set of entities that are flagged as killed or created, awaiting creation and destruction in the next registry update
		std::set<Entity> createdEntities;
		std::set<Entity> killedEntities;

		// Map of active systems (index = system typeid)
		std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	public:
		Registry() = default;

		// Entity management
		Entity CreateEntity();
		void KillEntity(Entity entity);    // flag entities to be destroyed in the next update
		void DestroyEntity(Entity entity); // this effectively removes the recently killed entities from the scene

		// Updates the systems so that created/deleted entities are removed from the systems' vectors of entities.
		void Update();

		// System management
		template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
		template <typename TSystem> void RemoveSystem();
		template <typename TSystem> bool HasSystem() const;
		template <typename TSystem> TSystem& GetSystem() const;

		/* Checks entity component signatures and adds an the entity to each system that is interested in it */
		void AddEntityToSystems(Entity entity);

		template <typename TComponent> std::shared_ptr<Pool<TComponent>> AccommodateComponent();
		const Signature& GetComponentSignature(Entity entity) const;

		/* Component management. */
		template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
		template <typename TComponent> void RemoveComponent(Entity entity);
		template <typename TComponent> bool HasComponent(Entity entity) const;
		template <typename TComponent> TComponent& GetComponent(Entity entity) const;
};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs && ...args) {
	if (HasSystem<TSystem>()) {
		return;
	}
	std::shared_ptr<TSystem> newSystem(new TSystem(std::forward<TArgs>(args) ...));
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem() {
	if (!HasSystem<TSystem>()) {
		return;
	}
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TSystem>
bool Registry::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	
	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool;
	componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args) ...);

	componentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	return componentPool->Get(entityId);
}

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
	registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
	return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
	return registry->GetComponent<TComponent>(*this);
}

#endif