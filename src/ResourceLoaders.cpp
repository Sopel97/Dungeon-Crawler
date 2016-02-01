#include "ResourceLoaders.h"

std::pair<std::string, sf::Texture*> ResourceLoader<sf::Texture>::load(const std::string& path)
{
    sf::Texture* texture = new sf::Texture;
    if(!texture->loadFromFile(path))
    {
        texture->setSmooth(true); //not sure
        return std::make_pair(path, nullptr);
    }
    return std::make_pair(path, texture);
}

std::pair<std::string, Tile*> ResourceLoader<Tile>::load(const std::string& path)
{

    Configuration config = Configuration(path);
    ConfigurationNode tileConfig = config["tile"];

    std::string tileName = tileConfig["name"].get<std::string>();

    std::unique_ptr<TileModel> model = nullptr;
    std::unique_ptr<TileView> view = nullptr;
    std::unique_ptr<TileController> controller = nullptr;

    std::string modelName = tileConfig["model"].get<std::string>();
    try
    {
        model = tileModels().at(modelName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No tile model with name " + modelName);
    }

    std::string viewName = tileConfig["view"].get<std::string>();
    try
    {
        view = tileViews().at(viewName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No tile view with name " + viewName);
    }

    std::string controllerName = tileConfig["controller"].get<std::string>();
    try
    {
        controller = tileControllers().at(controllerName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No tile controller with name " + controllerName);
    }

    std::unique_ptr<Tile> tile = std::make_unique<Tile>
        (
            std::move(model),
            std::move(view),
            std::move(controller)
            );

    tile->loadFromConfiguration(tileConfig);

    std::cout << "\nLoaded tile: " << tileName << "\n      model: " << modelName << "\n       view: " << viewName << "\n controller: " << controllerName << '\n';

    return std::make_pair(tileName, tile.release());
}

std::pair<std::string, Entity*> ResourceLoader<Entity>::load(const std::string& path)
{
    Configuration config = Configuration(path);
    ConfigurationNode entityConfig = config["entity"];

    std::string entityName = entityConfig["name"].get<std::string>();

    std::unique_ptr<EntityModel> model = nullptr;
    std::unique_ptr<EntityView> view = nullptr;
    std::unique_ptr<EntityController> controller = nullptr;

    std::string modelName = entityConfig["model"].get<std::string>();
    try
    {
        model = entityModels().at(modelName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No entity model with name " + modelName);
    }

    std::string viewName = entityConfig["view"].get<std::string>();
    try
    {
        view = entityViews().at(viewName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No entity view with name " + viewName);
    }

    std::string controllerName = entityConfig["controller"].get<std::string>();
    try
    {
        controller = entityControllers().at(controllerName)->create(nullptr);
    }
    catch(std::out_of_range&)
    {
        throw std::runtime_error("No entity controller with name " + controllerName);
    }

    std::unique_ptr<Entity> entity = std::make_unique<Entity>
        (
            std::move(model),
            std::move(view),
            std::move(controller)
            );

    entity->loadFromConfiguration(entityConfig);

    std::cout << "\nLoaded entity: " << entityName << "\n        model: " << modelName << "\n         view: " << viewName << "\n   controller: " << controllerName << '\n';

    return std::make_pair(entityName, entity.release());
}

std::pair<std::string, sf::Font*> ResourceLoader<sf::Font>::load(const std::string& path)
{
    sf::Font* font = new sf::Font;
    if(!font->loadFromFile(path)) return std::make_pair(path, nullptr);
    return std::make_pair(path, font);
}