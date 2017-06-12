#pragma once

#include <memory>
#include <vector>

class ComponentCommonData;

template <class...>
class ComponentFactory
{
};

template <class OwnerType, class BaseComponent>
class ComponentFactory<OwnerType, BaseComponent>
{
public:
    virtual std::unique_ptr<BaseComponent> create(OwnerType&, ComponentCommonData*) const = 0;
    virtual ComponentCommonData* createCommonDataStorage() const = 0;
};

// stores all created common datas
// all component factories are 'almost global' (they originate from singleton) so they outlive everything
template <class OwnerType, class BaseComponent, class DerivedComponent>
class ComponentFactory<OwnerType, BaseComponent, DerivedComponent> : public ComponentFactory<OwnerType, BaseComponent>
{
public:
    std::unique_ptr<BaseComponent> create(OwnerType& owner, ComponentCommonData* commonData) const override
    {
        return std::make_unique<DerivedComponent>(owner, commonData);
    }

    ComponentCommonData* createCommonDataStorage() const override
    {
        std::unique_ptr<ComponentCommonData> commonData = DerivedComponent::createCommonDataStorage();
        m_commonDataStorages.emplace_back(std::move(commonData));
        return m_commonDataStorages.back().get();
    }

private:
    mutable std::vector<std::unique_ptr<ComponentCommonData>> m_commonDataStorages;
};