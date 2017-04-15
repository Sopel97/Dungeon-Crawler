#pragma once

#include "ResourceManager.h"

#include "tiles/Tile.h"
#include "tiles/TilePrefab.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
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
class TileRenderer;
class TileController;

class Entity;
class EntityModel;
class EntityRenderer;
class EntityController;

template <>
class ResourceLoader<sf::Texture>
{
public:
    static std::pair<std::string, std::unique_ptr<sf::Texture>> load(const std::string& path); //should return nullptr when resource was not loaded
};

#define REGISTER_TILE_MODEL_TYPE(TYPE) \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_TILE_RENDERER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_TILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileControllerTypeRegistrar<TYPE> TYPE ## _controller_var (#TYPE); \
    }

template <>
class ResourceLoader<TilePrefab>
{
public:

    template<class T>
    struct TileModelTypeRegistrar
    {
        TileModelTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile model type: " << name << '\n';
            ResourceLoader<TilePrefab>::tileModels().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct TileRendererTypeRegistrar
    {
        TileRendererTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile renderer type: " << name << '\n';
            ResourceLoader<TilePrefab>::tileRenderers().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct TileControllerTypeRegistrar
    {
        TileControllerTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered tile controller type: " << name << '\n';
            ResourceLoader<TilePrefab>::tileControllers().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<TilePrefab>> load(const std::string& path); //should return nullptr when resource was not loaded
protected:

    static std::map<std::string, std::unique_ptr<TileModel>>& tileModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::unique_ptr<TileModel>> _tileModels;
        return _tileModels;
    }
    static std::map<std::string, std::unique_ptr<TileRenderer>>& tileRenderers()
    {
        static std::map<std::string, std::unique_ptr<TileRenderer>> _tileRenderers;
        return _tileRenderers;
    }
    static std::map<std::string, std::unique_ptr<TileController>>& tileControllers()
    {
        static std::map<std::string, std::unique_ptr<TileController>> _tileControllers;
        return _tileControllers;
    }

};



#define REGISTER_ENTITY_MODEL_TYPE(TYPE) \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Entity>::EntityModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_ENTITY_RENDERER_TYPE(TYPE)  \
    namespace ___TypeRegistering \
    { \
        const ResourceLoader<Entity>::EntityRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
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
            ResourceLoader<Entity>::entityModels().insert(std::make_pair(name, std::make_unique<T>(nullptr)));
        }
    };

    template<class T>
    struct EntityRendererTypeRegistrar
    {
        EntityRendererTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered entity renderer type: " << name << '\n';
            ResourceLoader<Entity>::entityRenderers().insert(std::make_pair(name, std::make_unique<T>(nullptr)));
        }
    };

    template<class T>
    struct EntityControllerTypeRegistrar
    {
        EntityControllerTypeRegistrar(const std::string& name)
        {
            std::cout << "Registered entity controller type: " << name << '\n';
            ResourceLoader<Entity>::entityControllers().insert(std::make_pair(name, std::make_unique<T>(nullptr)));
        }
    };

    static std::pair<std::string, std::unique_ptr<Entity>> load(const std::string& path); //should return nullptr when resource was not loaded

protected:

    static std::map<std::string, std::unique_ptr<EntityModel>>& entityModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::unique_ptr<EntityModel>> _entityModels;
        return _entityModels;
    }
    static std::map<std::string, std::unique_ptr<EntityRenderer>>& entityRenderers()
    {
        static std::map<std::string, std::unique_ptr<EntityRenderer>> _entityRenderers;
        return _entityRenderers;
    }
    static std::map<std::string, std::unique_ptr<EntityController>>& entityControllers()
    {
        static std::map<std::string, std::unique_ptr<EntityController>> _entityControllers;
        return _entityControllers;
    }
};

template <>
class ResourceLoader<sf::Font>
{
public:
    static std::pair<std::string, std::unique_ptr<sf::Font>> load(const std::string& path); //should return nullptr when resource was not loaded
};
