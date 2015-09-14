#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include <memory>

class Tile;
class TileLocation;
class Inventory;

class TileModel //must be functions (ie. all methods return resonable values and there is not pure virtual member functions)
{
public:
    TileModel(Tile* owner);
    TileModel(const TileModel& other);
    virtual ~TileModel();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual bool equals(const TileModel& other) const; //NOTE: it is guaranteed that the type of other is the same as the type of *this

    const Tile* owner() const;

    void setOwner(Tile* newOwner);

    virtual bool hasCollider() const;
    virtual const ls::RectangleF& collider() const;

    virtual Inventory* inventory();

    virtual float drag() const; //TODO: make it possible to specify that tile uses the drag of the tile lower

    virtual void onTilePlaced(const TileLocation& location);
    virtual void onTileRemoved(const TileLocation& location);
    virtual void onTileQuantityChanged(int newQuantity);

    virtual std::unique_ptr<TileModel> clone() const;
    virtual std::unique_ptr<TileModel> create(Tile* owner) const;
protected:
    Tile* m_owner;

private:
    static const ls::RectangleF m_emptyCollider;
};

#endif // TILEMODEL_H
