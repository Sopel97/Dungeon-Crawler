#pragma once

#include <memory>

#include "Component.h"

#include "../LibS/Shapes.h"

class ComponentCommonData;

template <class ComponentType, class OwnerType>
class EntityComponent : public Component<ComponentType, OwnerType>
{
public:
    EntityComponent(OwnerType& owner) : Component<ComponentType, OwnerType>(owner) {};
    EntityComponent(const EntityComponent& other, OwnerType& owner) : Component<ComponentType, OwnerType>(other, owner) {};
    ~EntityComponent() override = default;

    virtual void onEntityInstantiated(const ls::Vec2F& pos) {}
    virtual void onEntityCloned() {}
};