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
    virtual std::unique_ptr<BaseComponent> createWithNewCommonData(OwnerType&) const = 0;
};

// stores all created common datas
// all component factories are 'almost global' (they originate from singleton) so they outlive everything
template <class OwnerType, class BaseComponent, class DerivedComponent>
class ComponentFactory<OwnerType, BaseComponent, DerivedComponent> : public ComponentFactory<OwnerType, BaseComponent>
{
public:
    using CommonDataType = typename DerivedComponent::CommonData;

    std::unique_ptr<BaseComponent> createWithNewCommonData(OwnerType& owner) const override
    {
        return createWithNewCommonDataHelper<CommonDataType>(owner);
    }

private:
    template <class C>
    std::unique_ptr<BaseComponent> createWithNewCommonDataHelper(OwnerType& owner) const
    {
        return std::make_unique<DerivedComponent>(owner, *(createCommonDataStorage<C>()));
    }
    template <>
    std::unique_ptr<BaseComponent> createWithNewCommonDataHelper<void>(OwnerType& owner) const
    {
        return std::make_unique<DerivedComponent>(owner);
    }

    template <class C>
    CommonDataType* createCommonDataStorage() const
    {
        std::unique_ptr<CommonDataType> commonData = std::make_unique<CommonDataType>();

        m_cont.commonDataStorages.emplace_back(std::move(commonData));
        return m_cont.commonDataStorages.back().get();
    }
    template <>
    CommonDataType* createCommonDataStorage<void>() const
    {
        return nullptr;
    }

    template <class C>
    struct CommonDataStoragesContainer
    {
        std::vector<std::unique_ptr<C>> commonDataStorages;
    };

    template <>
    struct CommonDataStoragesContainer<void>
    {

    };

    mutable CommonDataStoragesContainer<CommonDataType> m_cont;
};
