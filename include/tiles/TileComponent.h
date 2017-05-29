#pragma once

#include <memory>

#include "Component.h"

class ComponentCommonData;

struct LuaTableNode;
typedef LuaTableNode ConfigurationNode;

template <class ComponentType, class OwnerType>
class TileComponent : public Component<ComponentType, OwnerType>
{
public:
    TileComponent() = default;
    TileComponent(const TileComponent& other) = default;
    virtual ~TileComponent() = default;

    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity) {}
    virtual void onTileInstantiated() {}
    virtual void onTileCloned() {}
};