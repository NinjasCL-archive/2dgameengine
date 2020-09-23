#include <iostream>
#include <fstream>
#include "./Game.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include <glm/glm.hpp>

int Game::windowWidth;
int Game::windowHeight;

Game::Game() {
    SDL_Log("Game constructor invoked");
    isRunning = false;
    ticksPreviousFrame = 0;
}

Game::~Game() {
    SDL_Log("Game destructor invoked");
}

bool Game::IsRunning() const {
    return isRunning;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN: {
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
                break;
            }
            case SDL_KEYUP: {
                eventBus->EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
                break;
            }
        }
    }
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    // Initialize the managers for the eventbus, assetstore, and ecs registry
    eventBus = std::make_unique<EventBus>();
    assetStore = std::make_unique<AssetStore>();
    registry = std::make_unique<Registry>();

    LoadAssets();
    LoadTileMap("./assets/tilemaps/jungle.map", "tilemap-texture", 25, 20, 32, 2.0);
    LoadEntities();
    LoadSystems();

    isRunning = true;
    return;
}

void Game::LoadAssets() {
    assetStore->ClearAssets();
    assetStore->AddTexture(renderer, "tank-texture", "./assets/images/tank-big-right.png");
    assetStore->AddTexture(renderer, "truck-texture", "./assets/images/truck-left.png");
    assetStore->AddTexture(renderer, "chopper-texture", "./assets/images/chopper-spritesheet.png");
    assetStore->AddTexture(renderer, "bandit-texture", "./assets/images/bandit-spritesheet.png");
    assetStore->AddTexture(renderer, "base-texture", "./assets/images/base.png");
    assetStore->AddTexture(renderer, "radar-texture", "./assets/images/radar.png");
    assetStore->AddTexture(renderer, "bullet-texture", "./assets/images/bullet.png");
    assetStore->AddTexture(renderer, "tilemap-texture", "./assets/tilemaps/jungle.png");
}

void Game::LoadSystems() {
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<KeyboardControlSystem>();
}

void Game::LoadTileMap(std::string mapFilePath, std::string textureAssetId, int mapNumCols, int mapNumRows, int tileSize, double scale) {
    std::fstream mapFile;
    mapFile.open(mapFilePath);
    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (scale * tileSize), y * (scale * tileSize)), glm::vec2(scale, scale), 0.0);
            tile.AddComponent<SpriteComponent>(textureAssetId, tileSize, tileSize, 0, srcRectX, srcRectY);
        }
    }
    mapFile.close();
}

void Game::LoadEntities() {
    Entity base = registry->CreateEntity();
    base.AddComponent<TransformComponent>(glm::vec2(240, 115), glm::vec2(1, 1), 0.0);
    base.AddComponent<SpriteComponent>("base-texture", 32, 32, 1);

    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(800, 150), glm::vec2(1, 1), 0.0);
    tank.AddComponent<HealthComponent>(100);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(3, 0));
    tank.AddComponent<SpriteComponent>("tank-texture", 32, 32, 2);
    tank.AddComponent<BoxColliderComponent>(glm::vec2(0), 32, 32);

    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(400, 500), glm::vec2(1, 1), 0.0);
    truck.AddComponent<HealthComponent>(100);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(-5, 0));
    truck.AddComponent<SpriteComponent>("truck-texture", 32, 32, 2);
    truck.AddComponent<BoxColliderComponent>(glm::vec2(0), 32, 32);

    Entity chopper = registry->CreateEntity();
    chopper.AddComponent<TransformComponent>(glm::vec2(240, 108), glm::vec2(1, 1), 0.0);
    chopper.AddComponent<HealthComponent>(100);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
    chopper.AddComponent<SpriteComponent>("chopper-texture", 32, 32, 3);
    chopper.AddComponent<AnimationComponent>(2, 10);
    chopper.AddComponent<BoxColliderComponent>(glm::vec2(0), 32, 32);
    chopper.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -20), glm::vec2(20, 0), glm::vec2(0, 20), glm::vec2(-20, 0));

    Entity chopper2 = registry->CreateEntity();
    chopper2.AddComponent<TransformComponent>(glm::vec2(240, 160), glm::vec2(1, 1), 0.0);
    chopper2.AddComponent<HealthComponent>(100);
    chopper2.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
    chopper2.AddComponent<SpriteComponent>("bandit-texture", 32, 32, 3);
    chopper2.AddComponent<AnimationComponent>(2, 10);
    chopper2.AddComponent<BoxColliderComponent>(glm::vec2(0), 32, 32);
    chopper2.AddComponent<KeyboardControlledComponent>(glm::vec2(0, -15), glm::vec2(15, 0), glm::vec2(0, 15), glm::vec2(-15, 0));

    Entity radar = registry->CreateEntity();
    radar.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0.0);
    radar.AddComponent<SpriteComponent>("radar-texture", 64, 64, 9);
    radar.AddComponent<AnimationComponent>(8, 5);
}

void Game::Update() {
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - ticksPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }
    double deltaTime = (SDL_GetTicks() - ticksPreviousFrame) / 1000.0f;
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
    ticksPreviousFrame = SDL_GetTicks();

    // Reset all event handlers for the current frame
    eventBus->Reset();

    // Perform the subscription of events for all systems
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventBus);

    // Update and refresh all entities
    registry->Update();

    // Update all systems that should be executed in the current frame
    registry->GetSystem<MovementSystem>().Update(registry, deltaTime);
    registry->GetSystem<KeyboardControlSystem>().Update(registry, deltaTime);
    registry->GetSystem<AnimationSystem>().Update(registry, deltaTime);
    registry->GetSystem<CollisionSystem>().Update(registry, eventBus);
    registry->GetSystem<DamageSystem>().Update(registry);
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // Call the render system to draw the game objects in our SDL renderer
    registry->GetSystem<RenderSystem>().Update(registry, renderer, assetStore);
    registry->GetSystem<RenderColliderSystem>().Update(registry, renderer);

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
