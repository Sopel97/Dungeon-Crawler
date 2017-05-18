#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <memory>

#include "Component.h"

class Tile;
class TileLocation;
class Inventory;

class TileModel : public Component<TileModel, Tile>
	//must be functional (ie. all methods return resonable values and there is no pure virtual member functions)
{
public:
    TileModel();
    TileModel(const TileModel& other);
    virtual ~TileModel();

    virtual bool equals(const TileModel& other) const; //NOTE: assume that the type of other is the same as the type of *this

    virtual bool hasCollider() const;
    virtual const ls::Rectangle2F& collider() const;
    virtual bool isMovableFrom() const;
    virtual bool isMovableTo() const;

    virtual Inventory* inventory();
    virtual const Inventory* inventory() const;

    virtual float drag() const; //TODO: make it possible to specify that tile uses the drag of the tile lower

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInstantiated();
    virtual void onTileCloned();

    virtual std::unique_ptr<TileModel> clone() const;

private:
    static const ls::Rectangle2F m_emptyCollider;
};

#endif // TILEMODEL_H
