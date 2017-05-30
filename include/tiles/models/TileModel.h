#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <memory>

#include "tiles/TileComponent.h"

#include "SlotContentRequirement.h"

class Tile;
class TileLocation;
class Inventory;

class TileModel : public TileComponent<TileModel, Tile>
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
    virtual bool isThrowableThrough() const;
    virtual bool isMovableTo() const;
    virtual int maxThrowDistance() const;
    virtual bool canBeStored() const;
    virtual int maxQuantity() const;
    virtual bool meetsRequirements(SlotContentRequirement req) const;

    virtual Inventory* inventory();
    virtual const Inventory* inventory() const;

    virtual float drag() const; //TODO: make it possible to specify that tile uses the drag of the tile lower

    virtual std::unique_ptr<TileModel> clone() const;

private:
    static const ls::Rectangle2F m_emptyCollider;
};

#endif // TILEMODEL_H
