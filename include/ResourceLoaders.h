#ifndef RESOURCELOADERS_H_INCLUDED
#define RESOURCELOADERS_H_INCLUDED

#include "ResourceManager.h"

#include <memory>
#include <iostream>

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

#define REGISTER_TILE_MODEL_TYPE(TYPE) \
    namespace ___TypeRegistering \
    { \
        TileLoader::TileModelTypeRegistrar<TYPE> TYPE ## _var (#TYPE); \
    } \

#define REGISTER_TILE_VIEW_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        TileLoader::TileViewTypeRegistrar<TYPE> TYPE ## _var (#TYPE); \
    } \

#define REGISTER_TILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        TileLoader::TileControllerTypeRegistrar<TYPE> TYPE ## _var (#TYPE); \
    } \

class TileLoader : public ResourceLoader
{
public:

    template<class T>
    struct TileModelTypeRegistrar
    {
        TileModelTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile model type: " << name << '\n';
            TileLoader::tileModels().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct TileViewTypeRegistrar
    {
        TileViewTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile view type: " << name << '\n';
            TileLoader::tileViews().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct TileControllerTypeRegistrar
    {
        TileControllerTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile controller type: " << name << '\n';
            TileLoader::tileControllers().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    TileLoader();
    typedef Tile LoadType;
    virtual std::pair<std::string, void*> load(const std::string& path) const; //should return nullptr when resource was not loaded
    virtual ~TileLoader();



protected:

    static std::map<std::string, std::shared_ptr<TileModel>>& tileModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::shared_ptr<TileModel>> _tileModels;
        return _tileModels;
    }
    static std::map<std::string, std::shared_ptr<TileView>>& tileViews()
    {
        static std::map<std::string, std::shared_ptr<TileView>> _tileViews;
        return _tileViews;
    }
    static std::map<std::string, std::shared_ptr<TileController>>& tileControllers()
    {
        static std::map<std::string, std::shared_ptr<TileController>> _tileControllers;
        return _tileControllers;
    }

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
