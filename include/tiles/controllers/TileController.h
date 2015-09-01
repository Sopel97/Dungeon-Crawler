#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include "Configuration.h"

#include <memory>

class Tile;
class TileLocation;
class Player;

class TileController //must not be abstract (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileController(Tile* owner);
    TileController(const TileController& other);
    virtual ~TileController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int newQuantity);
    virtual void onTileUsedByPlayer(const TileLocation& location, Player& player);

    virtual std::unique_ptr<TileController> clone() const;
    virtual std::unique_ptr<TileController> create(Tile* owner) const;
protected:
    Tile* m_owner;
};

#endif // TILECONTROLLER_H
