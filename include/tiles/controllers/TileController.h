#ifndef TILECONTROLLER_H
#define TILECONTROLLER_H

#include "Configuration.h"

#include "../../ComponentCommonData.h"

#include <memory>

class Tile;
class TileLocation;
class Player;

class TileController //must not be abstract (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileController();
    TileController(const TileController& other);
    virtual ~TileController();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    void setOwner(Tile* newOwner);

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileUsedByPlayer(const TileLocation& location);
    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileController> clone() const;
protected:
    Tile* m_owner;
};

#endif // TILECONTROLLER_H
