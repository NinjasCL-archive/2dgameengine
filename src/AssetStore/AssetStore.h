#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL.h>

class AssetStore {
    private:
        std::map<std::string, SDL_Texture*> textures;
    
    public:
        AssetStore();
        ~AssetStore();
        void ClearAssets();
        void AddTexture(SDL_Renderer* renderer, std::string assetId, std::string filePath);
        SDL_Texture* GetTexture(std::string assetId);
};

#endif
