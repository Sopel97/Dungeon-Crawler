#ifndef TILEMODEL_H
#define TILEMODEL_H

#include "Configuration.h"

#include "../LibS/Geometry.h"

#include "ComponentCommonData.h"

#include <memory>
#include <optional>

#include "tiles/TileComponent.h"

#include "SlotContentRequirement.h"

#include "colliders/TileCollider.h"

class Tile;
class TileLocation;
class Inventory;
class AttributeSet;
class TileInformation;

class TileModel : public TileComponent<TileModel, Tile>
	//must be functional (ie. all methods return resonable values and there is no pure virtual member functions)
{
public:
    TileModel(Tile& owner);
    TileModel(const TileModel& other, Tile& owner);
    ~TileModel() override;

    virtual bool equals(const TileModel& other) const; //NOTE: assume that the type of other is the same as the type of *this

    virtual std::optional<TileCollider> collider(const ls::Vec2I& pos);
    virtual bool isMovableFrom() const;
    virtual bool isThrowableThrough() const;
    virtual bool isMovableTo() const;
    virtual int maxThrowDistance() const;
    virtual bool canBeStored() const;
    virtual int maxQuantity() const;
    virtual bool meetsRequirements(SlotContentRequirement req) const;
    virtual const std::string& displayedName() const;
    virtual const AttributeSet& attributes() const;
    virtual TileInformation additionalInformation() const;

    virtual Inventory* inventory();
    virtual const Inventory* inventory() const;

    virtual float drag() const; //TODO: make it possible to specify that tile uses the drag of the tile lower

    virtual std::unique_ptr<TileModel> clone(Tile& owner) const;

private:
};

#endif // TILEMODEL_H
