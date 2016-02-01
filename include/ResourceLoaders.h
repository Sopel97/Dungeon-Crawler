#ifndef RESOURCELOADERS_H_INCLUDED
#define RESOURCELOADERS_H_INCLUDED

#include "ResourceManager.h"

#include "tiles/Tile.h"
#include "tiles/models/TileModel.h"
#include "tiles/views/TileView.h"
#include "tiles/controllers/TileController.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "entities/views/EntityView.h"
#include "entities/controllers/EntityController.h"

#include "Configuration.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <map>
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

class Entity;
class EntityModel;
class EntityView;
class EntityController;

template <>
class ResourceLoader<sf::Texture>
{
public:
    static std::pair<std::string, sf::Texture*> load(const std::string& path); //should return nullptr when resource was not loaded
};

#define REGISTER_TILE_MODEL_TYPE(TYPE) \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Tile>::TileModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_TILE_VIEW_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Tile>::TileViewTypeRegistrar<TYPE> TYPE ## _view_var (#TYPE); \
    }

#define REGISTER_TILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Tile>::TileControllerTypeRegistrar<TYPE> TYPE ## _controller_var (#TYPE); \
    }

template <>
class ResourceLoader<Tile>
{
public:

    template<class T>
    struct TileModelTypeRegistrar
    {
        TileModelTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile model type: " << name << '\n';
            ResourceLoader<Tile>::tileModels().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct TileViewTypeRegistrar
    {
        TileViewTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile view type: " << name << '\n';
            ResourceLoader<Tile>::tileViews().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct TileControllerTypeRegistrar
    {
        TileControllerTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile controller type: " << name << '\n';
            ResourceLoader<Tile>::tileControllers().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    static std::pair<std::string, Tile*> load(const std::string& path); //should return nullptr when resource was not loaded
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



#define REGISTER_ENTITY_MODEL_TYPE(TYPE) \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Entity>::EntityModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_ENTITY_VIEW_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Entity>::EntityViewTypeRegistrar<TYPE> TYPE ## _view_var (#TYPE); \
    }

#define REGISTER_ENTITY_CONTROLLER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Entity>::EntityControllerTypeRegistrar<TYPE> TYPE ## _controller_var (#TYPE); \
    } 

template <>
class ResourceLoader<Entity>
{
public:

    template<class T>
    struct EntityModelTypeRegistrar
    {
        EntityModelTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered entity model type: " << name << '\n';
            ResourceLoader<Entity>::entityModels().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct EntityViewTypeRegistrar
    {
        EntityViewTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered entity view type: " << name << '\n';
            ResourceLoader<Entity>::entityViews().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    template<class T>
    struct EntityControllerTypeRegistrar
    {
        EntityControllerTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered entity controller type: " << name << '\n';
            ResourceLoader<Entity>::entityControllers().insert(std::make_pair(name, std::make_shared<T>(nullptr)));
        }
    };

    static std::pair<std::string, Entity*> load(const std::string& path); //should return nullptr when resource was not loaded

protected:

    static std::map<std::string, std::shared_ptr<EntityModel>>& entityModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::shared_ptr<EntityModel>> _entityModels;
        return _entityModels;
    }
    static std::map<std::string, std::shared_ptr<EntityView>>& entityViews()
    {
        static std::map<std::string, std::shared_ptr<EntityView>> _entityViews;
        return _entityViews;
    }
    static std::map<std::string, std::shared_ptr<EntityController>>& entityControllers()
    {
        static std::map<std::string, std::shared_ptr<EntityController>> _entityControllers;
        return _entityControllers;
    }
};

template <>
class ResourceLoader<sf::Font>
{
public:
    static std::pair<std::string, sf::Font*> load(const std::string& path); //should return nullptr when resource was not loaded
};


#endif // RESOURCELOADERS_H_INCLUDED
