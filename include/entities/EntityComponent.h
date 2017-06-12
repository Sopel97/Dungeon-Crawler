#pragma once

#include <memory>

#include "Component.h"

class ComponentCommonData;

template <class ComponentType, class OwnerType>
class EntityComponent : public Component<ComponentType, OwnerType>
{
public:
    EntityComponent(OwnerType& owner) : Component<ComponentType, OwnerType>(owner) {};
    EntityComponent(const EntityComponent& other, OwnerType& owner) : Component<ComponentType, OwnerType>(other, owner) {};
    ~EntityComponent() override = default;

    virtual void onEntityInstantiated() {}
    virtual void onEntityCloned() {}
};