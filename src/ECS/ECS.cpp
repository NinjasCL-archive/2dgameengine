#include "ECS.h"

#include <iostream>

int IComponent::nextId = 0;

int Entity::GetId() const {
	return id;
}

void Entity::Kill() {
	registry->KillEntity(*this);
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	entities.erase(std::remove_if(entities.begin(), entities.end(),	[&entity](Entity other) {
		return entity == other;
	}), entities.end());
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;

	// If there are no free ids waiting to be reused
	if (freeIds.empty()) {
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size()) {
			entityComponentSignatures.resize(entityId + 1);
		}
	} else {
		// Reuse an id from the list of previously removed ids
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId);
	entity.registry = this;

	createdEntities.insert(entity);

	std::cout << "Created Entity with ID " << entityId << " (entity count = " << numEntities << ")" << std::endl;

	return entity;
}

void Registry::DestroyEntity(Entity entity) {
	const int entityId = entity.GetId();
	
	// Make the id available for reuse
	freeIds.push_back(entityId);

	// Reset the component signature for that entity id
	entityComponentSignatures[entityId].reset();
	
	// Remove entity from all systems
	for (auto system: systems) {
		system.second->RemoveEntityFromSystem(entity);
	}
}

void Registry::KillEntity(Entity entity) {
	killedEntities.insert(entity);
}

void Registry::Update() {
	for (auto entity: createdEntities) {
		AddEntityToSystems(entity);
	}
	createdEntities.clear();

	for (auto entity: killedEntities) {
		DestroyEntity(entity);
	}
	killedEntities.clear();
}

const Signature& Registry::GetComponentSignature(Entity entity) const {
	const auto entityId = entity.GetId();
	return entityComponentSignatures[entityId];
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto &entityComponentSignature = GetComponentSignature(entity);
	for (auto &system: systems) {
		const auto &systemComponentSignature = system.second->GetComponentSignature();
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
		if (isInterested) {
			system.second->AddEntityToSystem(entity);
		}
	}
}