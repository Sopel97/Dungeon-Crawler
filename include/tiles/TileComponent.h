#pragma once

#include <memory>

#include "Component.h"

class ComponentCommonData;
class TileLocation;
class Inventory;

template <class ComponentType, class OwnerType>
class TileComponent : public Component<ComponentType, OwnerType>
{
public:
    TileComponent(OwnerType& owner) : Component<ComponentType, OwnerType>(owner) {};
    TileComponent(const TileComponent& other, OwnerType& owner) : Component<ComponentType, OwnerType>(other, owner) {};
    ~TileComponent() override = default;

    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity) {}
    virtual void onTileInstantiated() {}
    virtual void onTileCloned() {}

    virtual void onTilePlaced(const TileLocation& location) {}
    virtual void onTilePlaced(const Inventory& inventory, int slot) {}
    virtual void onTileRemoved(const TileLocation& location) {}
    virtual void onTileRemoved(const Inventory& inventory, int slot) {}
};