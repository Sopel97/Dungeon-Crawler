#include "ResourceLoaders.h"

#include "TileModel.h"

#include "TileView.h"

#include "TileController.h"

#include "Tile.h"

#include "Configuration.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <string>
#include <map>
#include "../LibS/make_unique.h"

std::pair<std::string, void*> TextureLoader::load(const std::string& path) const
{
    sf::Texture* texture = new sf::Texture;
    if(!texture->loadFromFile(path)) return std::make_pair(path, nullptr);
    return std::make_pair(path, texture);
}
TextureLoader::~TextureLoader()
{

}

TileLoader::TileLoader()
{
}
std::pair<std::string, void*> TileLoader::load(const std::string& path) const
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
    catch(std::out_of_range& e)
    {
        throw std::runtime_error("No tile model with name " + modelName);
    }

    std::string viewName = tileConfig["view"].get<std::string>();
    try
    {
        view = tileViews().at(viewName)->create(nullptr);
    }
    catch(std::out_of_range& e)
    {
        throw std::runtime_error("No tile view with name " + viewName);
    }

    std::string controllerName = tileConfig["controller"].get<std::string>();
    try
    {
        controller = tileControllers().at(controllerName)->create(nullptr);
    }
    catch(std::out_of_range& e)
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

    return std::make_pair(tileName, static_cast<void*>(tile.release()));
}

TileLoader::~TileLoader()
{

}


FontLoader::FontLoader()
{

}
std::pair<std::string, void*> FontLoader::load(const std::string& path) const
{
    sf::Font* font = new sf::Font;
    if(!font->loadFromFile(path)) return std::make_pair(path, nullptr);
    return std::make_pair(path, font);
}
FontLoader::~FontLoader()
{

}
