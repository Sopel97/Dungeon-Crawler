#pragma once

#include <memory>

template <class...>
class ComponentFactory
{
};

template <class OwnerType, class BaseComponent>
class ComponentFactory<OwnerType, BaseComponent>
{
public:
    virtual std::unique_ptr<BaseComponent> create(OwnerType&) const = 0;
};

template <class OwnerType, class BaseComponent, class DerivedComponent>
class ComponentFactory<OwnerType, BaseComponent, DerivedComponent> : public ComponentFactory<OwnerType, BaseComponent>
{
public:
    std::unique_ptr<BaseComponent> create(OwnerType& owner) const override
    {
        return std::make_unique<DerivedComponent>(owner);
    }
};