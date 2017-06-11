#pragma once

#include <memory>

#include "Component.h"

class ComponentCommonData;

template <class ComponentType, class OwnerType>
class EntityComponent : public Component<ComponentType, OwnerType>
{
public:
    EntityComponent() = default;
    EntityComponent(const EntityComponent& other) = default;
    ~EntityComponent() override = default;

    virtual void onEntityInstantiated() {}
    virtual void onEntityCloned() {}
};