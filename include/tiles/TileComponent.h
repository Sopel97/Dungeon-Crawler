#pragma once

#include <memory>

#include "Component.h"

class ComponentCommonData;

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
};