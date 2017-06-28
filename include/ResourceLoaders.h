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

#include "projectiles/Projectile.h"
#include "projectiles/ProjectilePrefab.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"
#include "projectiles/controllers/ProjectileController.h"

#include "Configuration.h"

#include "Logger.h"

#include "ComponentFactory.h"

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
    namespace ___TileModelTypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_TILE_RENDERER_TYPE(TYPE)  \
    namespace ___TileRendererTypeRegistering \
    { \
        const ResourceLoader<TilePrefab>::TileRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_TILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___TileControllerTypeRegistering \
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
            ResourceLoader<TilePrefab>::tileModels().insert(std::make_pair(name, std::make_unique<ComponentFactory<Tile, TileModel, T>>()));
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
            ResourceLoader<TilePrefab>::tileRenderers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Tile, TileRenderer, T>>()));
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
            ResourceLoader<TilePrefab>::tileControllers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Tile, TileController, T>>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<TilePrefab>> load(const std::string& path); //should return nullptr when resource was not loaded
protected:

    using TileModelFactory = ComponentFactory<Tile, TileModel>;
    using TileRendererFactory = ComponentFactory<Tile, TileRenderer>;
    using TileControllerFactory = ComponentFactory<Tile, TileController>;

    static std::map<std::string, std::unique_ptr<TileModelFactory>>& tileModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::unique_ptr<TileModelFactory>> _tileModels;
        return _tileModels;
    }
    static std::map<std::string, std::unique_ptr<TileRendererFactory>>& tileRenderers()
    {
        static std::map<std::string, std::unique_ptr<TileRendererFactory>> _tileRenderers;
        return _tileRenderers;
    }
    static std::map<std::string, std::unique_ptr<TileControllerFactory>>& tileControllers()
    {
        static std::map<std::string, std::unique_ptr<TileControllerFactory>> _tileControllers;
        return _tileControllers;
    }

};

#define REGISTER_ENTITY_MODEL_TYPE(TYPE) \
    namespace ___EntityModelTypeRegistering \
    { \
        const ResourceLoader<EntityPrefab>::EntityModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_ENTITY_RENDERER_TYPE(TYPE)  \
    namespace ___EntityRendererTypeRegistering \
    { \
        const ResourceLoader<EntityPrefab>::EntityRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_ENTITY_CONTROLLER_TYPE(TYPE)  \
    namespace ___EntityControllerTypeRegistering \
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
            ResourceLoader<EntityPrefab>::entityModels().insert(std::make_pair(name, std::make_unique<ComponentFactory<Entity, EntityModel, T>>()));
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
            ResourceLoader<EntityPrefab>::entityRenderers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Entity, EntityRenderer, T>>()));
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
            ResourceLoader<EntityPrefab>::entityControllers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Entity, EntityController, T>>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<EntityPrefab>> load(const std::string& path); //should return nullptr when resource was not loaded

protected:

    using EntityModelFactory = ComponentFactory<Entity, EntityModel>;
    using EntityRendererFactory = ComponentFactory<Entity, EntityRenderer>;
    using EntityControllerFactory = ComponentFactory<Entity, EntityController>;

    static std::map<std::string, std::unique_ptr<EntityModelFactory>>& entityModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::unique_ptr<EntityModelFactory>> _entityModels;
        return _entityModels;
    }
    static std::map<std::string, std::unique_ptr<EntityRendererFactory>>& entityRenderers()
    {
        static std::map<std::string, std::unique_ptr<EntityRendererFactory>> _entityRenderers;
        return _entityRenderers;
    }
    static std::map<std::string, std::unique_ptr<EntityControllerFactory>>& entityControllers()
    {
        static std::map<std::string, std::unique_ptr<EntityControllerFactory>> _entityControllers;
        return _entityControllers;
    }
};

#define REGISTER_PROJECTILE_MODEL_TYPE(TYPE) \
    namespace ___ProjectileModelTypeRegistering \
    { \
        const ResourceLoader<ProjectilePrefab>::ProjectileModelTypeRegistrar<TYPE> TYPE ## _model_var (#TYPE); \
    }

#define REGISTER_PROJECTILE_RENDERER_TYPE(TYPE)  \
    namespace ___ProjectileRendererTypeRegistering \
    { \
        const ResourceLoader<ProjectilePrefab>::ProjectileRendererTypeRegistrar<TYPE> TYPE ## _renderer_var (#TYPE); \
    }

#define REGISTER_PROJECTILE_CONTROLLER_TYPE(TYPE)  \
    namespace ___ProjectileControllerTypeRegistering \
    { \
        const ResourceLoader<ProjectilePrefab>::ProjectileControllerTypeRegistrar<TYPE> TYPE ## _controller_var (#TYPE); \
    } 

template <>
class ResourceLoader<ProjectilePrefab>
{
public:

    template<class T>
    struct ProjectileModelTypeRegistrar
    {
        ProjectileModelTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
                "Registered projectile model type: " + name;
            });
            ResourceLoader<ProjectilePrefab>::projectileModels().insert(std::make_pair(name, std::make_unique<ComponentFactory<Projectile, ProjectileModel, T>>()));
        }
    };

    template<class T>
    struct ProjectileRendererTypeRegistrar
    {
        ProjectileRendererTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
                "Registered projectile renderer type: " + name;
            });
            ResourceLoader<ProjectilePrefab>::projectileRenderers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Projectile, ProjectileRenderer, T>>()));
        }
    };

    template<class T>
    struct ProjectileControllerTypeRegistrar
    {
        ProjectileControllerTypeRegistrar(const std::string& name)
        {
            Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
                "Registered projectile controller type: " + name;
            });
            ResourceLoader<ProjectilePrefab>::projectileControllers().insert(std::make_pair(name, std::make_unique<ComponentFactory<Projectile, ProjectileController, T>>()));
        }
    };

    static std::pair<std::string, std::unique_ptr<ProjectilePrefab>> load(const std::string& path); //should return nullptr when resource was not loaded

protected:

    using ProjectileModelFactory = ComponentFactory<Projectile, ProjectileModel>;
    using ProjectileRendererFactory = ComponentFactory<Projectile, ProjectileRenderer>;
    using ProjectileControllerFactory = ComponentFactory<Projectile, ProjectileController>;

    static std::map<std::string, std::unique_ptr<ProjectileModelFactory>>& projectileModels() //to ensure that they are created during registation process. (When they are static members they get defined too late)
    {
        static std::map<std::string, std::unique_ptr<ProjectileModelFactory>> _projectileModels;
        return _projectileModels;
    }
    static std::map<std::string, std::unique_ptr<ProjectileRendererFactory>>& projectileRenderers()
    {
        static std::map<std::string, std::unique_ptr<ProjectileRendererFactory>> _projectileRenderers;
        return _projectileRenderers;
    }
    static std::map<std::string, std::unique_ptr<ProjectileControllerFactory>>& projectileControllers()
    {
        static std::map<std::string, std::unique_ptr<ProjectileControllerFactory>> _projectileControllers;
        return _projectileControllers;
    }
};

template <>
class ResourceLoader<sf::Font>
{
public:
    static std::pair<std::string, std::unique_ptr<sf::Font>> load(const std::string& path); //should return nullptr when resource was not loaded
};
