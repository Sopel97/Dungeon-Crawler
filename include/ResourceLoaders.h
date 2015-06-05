#ifndef RESOURCELOADERS_H_INCLUDED
#define RESOURCELOADERS_H_INCLUDED

#include "ResourceManager.h"

#include <memory>

namespace sf
{
    class Texture;
    class Font;
}
class Tile;
class TileModel;
class TileView;
class TileController;

class TextureLoader : public ResourceLoader
{
public:
    typedef sf::Texture LoadType;
    virtual std::pair<std::string, void*> load(const std::string& path) const; //should return nullptr when resource was not loaded
    virtual ~TextureLoader();
};


class TileLoader : public ResourceLoader
{
public:
    TileLoader();
    typedef Tile LoadType;
    virtual std::pair<std::string, void*> load(const std::string& path) const; //should return nullptr when resource was not loaded
    virtual ~TileLoader();
protected:
    std::map<std::string, std::unique_ptr<TileModel>> tileModels;
    std::map<std::string, std::unique_ptr<TileView>> tileViews;
    std::map<std::string, std::unique_ptr<TileController>> tileControllers;
};


class FontLoader : public ResourceLoader
{
public:
    FontLoader();
    typedef sf::Font LoadType;
    virtual std::pair<std::string, void*> load(const std::string& path) const; //should return nullptr when resource was not loaded
    virtual ~FontLoader();
};
#endif // RESOURCELOADERS_H_INCLUDED
