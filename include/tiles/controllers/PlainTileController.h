#ifndef PLAINTILECONTROLLER_H
#define PLAINTILECONTROLLER_H

#include "TileController.h"

#include "ResourceLoaders.h"

#include <memory>

class Tile;

class PlainTileController : public TileController
{
public:
    PlainTileController(Tile* owner);
    PlainTileController(const PlainTileController& other);
    virtual ~PlainTileController();

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);

    virtual std::unique_ptr<TileController> clone() const;
    virtual std::unique_ptr<TileController> create(Tile* owner) const;
};

REGISTER_TILE_CONTROLLER_TYPE(PlainTileController)

#endif // PLAINTILECONTROLLER_H
