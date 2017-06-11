#pragma once

#include "ResourceManager.h"

#include "tiles/Tile.h"
#include "tiles/TilePrefab.h"
#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

#include "entities/Entity.h"
#include "entities/EntityPrefab.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/controllers/EntityController.h"

#include "Configuration.h"

#include "Logger.h"

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

template <>
class ResourceLoader<sf::Texture>
{
public:
    static std::pair<std::string, std::unique_ptr<sf::Texture>> load(const std::string& path); //should return nullptr when resource was not loaded
};

#define REGISTER_TILE_MODEL_TYPE(TYPE) \
    namespace ___TileTypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_TILE_RENDERER_TYPE(TYPE)  \
    namespace ___TileTypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_TILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___TileTypeRegistering \
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
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered tile model type: " + name; 
            });
            ResourceLoader<TilePrefab>::tileModels().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct TileRendererTypeRegistrar
    {
        TileRendererTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered tile renderer type: " + name; 
            });
            ResourceLoader<TilePrefab>::tileRenderers().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct TileControllerTypeRegistrar
    {
        TileControllerTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered tile controller type: " + name; 
            });
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
    namespace ___EntityTypeRegistering \
    { \
        const ResourceLoader<EntityPrefab>::EntityModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_ENTITY_RENDERER_TYPE(TYPE)  \
    namespace ___EntityTypeRegistering \
    { \
        const ResourceLoader<EntityPrefab>::EntityRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_ENTITY_CONTROLLER_TYPE(TYPE)  \
    namespace ___EntityTypeRegistering \
    { \
        const ResourceLoader<EntityPrefab>::EntityControllerTypeRegistrar<TYPE> TYPE ## _controller_var (#TYPE); \
    } 

template <>
class ResourceLoader<EntityPrefab>
{
public:

    template<class T>
    struct EntityModelTypeRegistrar
    {
        EntityModelTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered entity model type: " + name; 
            });
            ResourceLoader<EntityPrefab>::entityModels().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct EntityRendererTypeRegistrar
    {
        EntityRendererTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered entity renderer type: " + name; 
            });
            ResourceLoader<EntityPrefab>::entityRenderers().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    template<class T>
    struct EntityControllerTypeRegistrar
    {
        EntityControllerTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
                "Registered entity controller type: " + name; 
            });
            ResourceLoader<EntityPrefab>::entityControllers().insert(std::make_pair(name, std::make_unique<T>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<EntityPrefab>> load(const std::string& path); //should return nullptr when resource was not loaded

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
