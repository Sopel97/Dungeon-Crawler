#include "ResourceLoaders.h"

#include "Logger.h"

std::pair<std::string, std::unique_ptr<Spritesheet>> ResourceLoader<Spritesheet>::load(const std::string& path, int gridSize, int padding, bool repeated)
{
    std::unique_ptr<Spritesheet> spritesheet = std::make_unique<Spritesheet>(path, gridSize, padding);
    spritesheet->setRepeated(repeated);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded texture: " + path + ";gridSize: " + std::to_string(gridSize) + ";padding: " + std::to_string(padding) + ";repeated: " + std::to_string(repeated);
    });

    return std::make_pair(path, std::move(spritesheet));
}

std::pair<std::string, std::unique_ptr<TilePrefab>> ResourceLoader<TilePrefab>::load(const std::string& path)
{
    Configuration config = Configuration(path);
    ConfigurationNode tileConfig = config["tile"];

    std::string tileName = tileConfig["name"].get<std::string>();

    ComponentFactory<Tile, TileModel>* modelFac = nullptr;
    ComponentFactory<Tile, TileRenderer>* rendererFac = nullptr;

    std::string modelName = tileConfig["model"].get<std::string>();
    try
    {
        modelFac = tileModels().at(modelName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile model with name " + modelName);
    }

    std::string rendererName = tileConfig["renderer"].get<std::string>();
    try
    {
        rendererFac = tileRenderers().at(rendererName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile renderer with name " + rendererName);
    }

    std::unique_ptr<TilePrefab> tile = std::make_unique<TilePrefab>(
        *modelFac,
        *rendererFac
    );

    tile->loadFromConfiguration(tileConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
        "Loaded tile: " + tileName + ";model: " + modelName + ";renderer: " + rendererName; 
    });

    return std::make_pair(tileName, std::move(tile));
}

std::pair<std::string, std::unique_ptr<EntityPrefab>> ResourceLoader<EntityPrefab>::load(const std::string& path)
{
    Configuration config = Configuration(path);
    ConfigurationNode entityConfig = config["entity"];

    std::string entityName = entityConfig["name"].get<std::string>();

    ComponentFactory<Entity, EntityModel>* modelFac = nullptr;
    ComponentFactory<Entity, EntityRenderer>* rendererFac = nullptr;

    std::string modelName = entityConfig["model"].get<std::string>();
    try
    {
        modelFac = entityModels().at(modelName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity model with name " + modelName);
    }

    std::string rendererName = entityConfig["renderer"].get<std::string>();
    try
    {
        rendererFac = entityRenderers().at(rendererName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity renderer with name " + rendererName);
    }


    std::unique_ptr<EntityPrefab> entity = std::make_unique<EntityPrefab>(
        *modelFac,
        *rendererFac
    );

    entity->loadFromConfiguration(entityConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded entity: " + entityName + ";model: " + modelName + ";renderer: " + rendererName; 
    });

    return std::make_pair(entityName, std::move(entity));
}

std::pair<std::string, std::unique_ptr<ProjectilePrefab>> ResourceLoader<ProjectilePrefab>::load(const std::string& path)
{
    Configuration config = Configuration(path);
    ConfigurationNode projectileConfig = config["projectile"];

    std::string projectileName = projectileConfig["name"].get<std::string>();

    ComponentFactory<Projectile, ProjectileModel>* modelFac = nullptr;
    ComponentFactory<Projectile, ProjectileRenderer>* rendererFac = nullptr;

    std::string modelName = projectileConfig["model"].get<std::string>();
    try
    {
        modelFac = projectileModels().at(modelName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No projectile model with name " + modelName);
    }

    std::string rendererName = projectileConfig["renderer"].get<std::string>();
    try
    {
        rendererFac = projectileRenderers().at(rendererName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No projectile renderer with name " + rendererName);
    }
    
    std::unique_ptr<ProjectilePrefab> projectile = std::make_unique<ProjectilePrefab>(
        *modelFac,
        *rendererFac
    );

    projectile->loadFromConfiguration(projectileConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded projectile: " + projectileName + ";model: " + modelName + ";renderer: " + rendererName;
    });

    return std::make_pair(projectileName, std::move(projectile));
}

std::pair<std::string, std::unique_ptr<sf::Font>> ResourceLoader<sf::Font>::load(const std::string& path)
{
    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(path)) return std::make_pair(path, nullptr);
    return std::make_pair(path, std::move(font));
}

std::pair<std::string, std::unique_ptr<EffectPrefab>> ResourceLoader<EffectPrefab>::load(const std::string& path)
{
    std::unique_ptr<EffectPrefab> effect = std::make_unique<EffectPrefab>();
    Configuration config = Configuration(path);
    ConfigurationNode effectConfig = config["effect"];
    std::string effectName = effectConfig["name"].get<std::string>();
    effect->loadFromConfiguration(effectConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded effect: " + effectName;
    });

    return std::make_pair(effectName, std::move(effect));
}