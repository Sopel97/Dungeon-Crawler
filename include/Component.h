#pragma once

#include <memory>

#include "ComponentCommonData.h"

class ComponentCommonData;

struct LuaTableNode;
typedef LuaTableNode ConfigurationNode;

template <class ComponentType, class OwnerType>
class Component
{
public:
    using CommonData = void;

    Component(OwnerType& owner) : m_owner(&owner) {}
    Component(const Component& other, OwnerType& owner) : m_owner(&owner) {}
	virtual ~Component() = default;

	virtual void loadFromConfiguration(ConfigurationNode& config)
	{

	}
	void setOwner(OwnerType* newOwner)
	{
		m_owner = newOwner;
	}
	const OwnerType& owner() const
	{
		return *m_owner;
	}
	OwnerType& owner()
	{
		return *m_owner;
	}

	virtual std::unique_ptr<ComponentType> clone(OwnerType& owner) const = 0;

protected:
	OwnerType* m_owner;
};