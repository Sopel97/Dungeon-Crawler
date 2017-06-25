#include "ResourceLoaders.h"

#include "Logger.h"

std::pair<std::string, std::unique_ptr<sf::Texture>> ResourceLoader<sf::Texture>::load(const std::string& path)
{
    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(path))
    {
        return std::make_pair(path, nullptr);
    }
    return std::make_pair(path, std::move(texture));
}

std::pair<std::string, std::unique_ptr<TilePrefab>> ResourceLoader<TilePrefab>::load(const std::string& path)
{
    Configuration config = Configuration(path);
    ConfigurationNode tileConfig = config["tile"];

    std::string tileName = tileConfig["name"].get<std::string>();

    ComponentFactory<Tile, TileModel>* modelFac = nullptr;
    ComponentFactory<Tile, TileRenderer>* rendererFac = nullptr;
    ComponentFactory<Tile, TileController>* controllerFac = nullptr;

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

    std::string controllerName = tileConfig["controller"].get<std::string>();
    try
    {
        controllerFac = tileControllers().at(controllerName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile controller with name " + controllerName);
    }

    std::unique_ptr<TilePrefab> tile = std::make_unique<TilePrefab>
        (
            *modelFac,
            *rendererFac,
            *controllerFac
            );

    tile->loadFromConfiguration(tileConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return 
        "Loaded tile: " + tileName + ";model: " + modelName + ";renderer: " + rendererName + "; controller: " + controllerName; 
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
    ComponentFactory<Entity, EntityController>* controllerFac = nullptr;

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

    std::string controllerName = entityConfig["controller"].get<std::string>();
    try
    {
        controllerFac = entityControllers().at(controllerName).get();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity controller with name " + controllerName);
    }

    std::unique_ptr<EntityPrefab> entity = std::make_unique<EntityPrefab>
        (
            *modelFac,
            *rendererFac,
            *controllerFac
            );

    entity->loadFromConfiguration(entityConfig);

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        "Loaded entity: " + entityName + ";model: " + modelName + ";renderer: " + rendererName + "; controller: " + controllerName; 
    });

    return std::make_pair(entityName, std::move(entity));
}

std::pair<std::string, std::unique_ptr<sf::Font>> ResourceLoader<sf::Font>::load(const std::string& path)
{
    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(path)) return std::make_pair(path, nullptr);
    return std::make_pair(path, std::move(font));
}