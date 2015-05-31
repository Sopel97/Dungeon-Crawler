#include "ResourceLoaders.h"

#include "TileModel.h"
#include "PlainTileModel.h"

#include "TileView.h"
#include "PlainTileView.h"

#include "TileController.h"
#include "PlainTileController.h"

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
    texture->loadFromFile(path);
    return std::make_pair(path, texture);
}
TextureLoader::~TextureLoader()
{

}


TileLoader::TileLoader()
{
    std::pair<std::string, std::unique_ptr<TileModel>> _tileModels[] =
    {
        {"PlainTileModel", std::make_unique<PlainTileModel>(nullptr)}
    };

    std::pair<std::string, std::unique_ptr<TileView>> _tileViews[] =
    {
        {"PlainTileView", std::make_unique<PlainTileView>(nullptr)}
    };

    std::pair<std::string, std::unique_ptr<TileController>> _tileControllers[] =
    {
        {"PlainTileController", std::make_unique<PlainTileController>(nullptr)}
    };

    tileModels = std::map<std::string, std::unique_ptr<TileModel>> {std::make_move_iterator(std::begin(_tileModels)),  std::make_move_iterator(std::end(_tileModels))};
    tileViews = std::map<std::string, std::unique_ptr<TileView>> {std::make_move_iterator(std::begin(_tileViews)),  std::make_move_iterator(std::end(_tileViews))};
    tileControllers = std::map<std::string, std::unique_ptr<TileController>> {std::make_move_iterator(std::begin(_tileControllers)),  std::make_move_iterator(std::end(_tileControllers))};
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
        model = tileModels.at(modelName)->clone();
    }
    catch(std::out_of_range& e)
    {
        throw std::runtime_error("No tile model with name " + modelName);
    }

    std::string viewName = tileConfig["view"].get<std::string>();
    try
    {
        view = tileViews.at(viewName)->clone();
    }
    catch(std::out_of_range& e)
    {
        throw std::runtime_error("No tile view with name " + viewName);
    }

    std::string controllerName = tileConfig["controller"].get<std::string>();
    try
    {
        controller = tileControllers.at(controllerName)->clone();
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

    return std::make_pair(tileName, static_cast<void*>(tile.release()));
}

TileLoader::~TileLoader()
{

}
