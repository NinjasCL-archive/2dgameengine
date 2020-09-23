#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

inline constexpr unsigned int FPS = 60;
inline constexpr unsigned int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    private:
        bool isRunning;
        bool showBoundingBox;
        int ticksPreviousFrame;
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Rect camera;

        std::unique_ptr<AssetStore> assetStore;
        std::unique_ptr<Registry> registry;
        std::unique_ptr<EventBus> eventBus;

    public:
        Game();
        ~Game();
        bool IsRunning() const;
        void Initialize();
        void ProcessInput();
        void Update();
        void Render();
        void LoadAssets();
        void LoadSystems();
        void LoadEntities();
        void LoadTileMap(std::string mapFilePath, std::string textureAssetId, int mapNumCols, int mapNumRows, int tileSize, double scale);
        void Destroy();

        static int windowWidth;
        static int windowHeight;
        static int mapWidth;
        static int mapHeight;
};

#endif
