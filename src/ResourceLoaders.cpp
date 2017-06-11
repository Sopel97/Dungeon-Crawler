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

    std::unique_ptr<TileModel> model = nullptr;
    std::unique_ptr<TileRenderer> renderer = nullptr;
    std::unique_ptr<TileController> controller = nullptr;

    std::string modelName = tileConfig["model"].get<std::string>();
    try
    {
        model = tileModels().at(modelName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile model with name " + modelName);
    }

    std::string rendererName = tileConfig["renderer"].get<std::string>();
    try
    {
        renderer = tileRenderers().at(rendererName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile renderer with name " + rendererName);
    }

    std::string controllerName = tileConfig["controller"].get<std::string>();
    try
    {
        controller = tileControllers().at(controllerName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No tile controller with name " + controllerName);
    }

    std::unique_ptr<TilePrefab> tile = std::make_unique<TilePrefab>
        (
            std::move(model),
            std::move(renderer),
            std::move(controller)
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

    std::unique_ptr<EntityModel> model = nullptr;
    std::unique_ptr<EntityRenderer> renderer = nullptr;
    std::unique_ptr<EntityController> controller = nullptr;

    std::string modelName = entityConfig["model"].get<std::string>();
    try
    {
        model = entityModels().at(modelName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity model with name " + modelName);
    }

    std::string rendererName = entityConfig["view"].get<std::string>();
    try
    {
        renderer = entityRenderers().at(rendererName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity renderer with name " + rendererName);
    }

    std::string controllerName = entityConfig["controller"].get<std::string>();
    try
    {
        controller = entityControllers().at(controllerName)->clone();
    }
    catch (std::out_of_range&)
    {
        throw std::runtime_error("No entity controller with name " + controllerName);
    }

    std::unique_ptr<EntityPrefab> entity = std::make_unique<EntityPrefab>
        (
            std::move(model),
            std::move(renderer),
            std::move(controller)
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