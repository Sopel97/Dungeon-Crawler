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
	Component() = default;
	Component(const Component& other) = default;
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

	virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const
	{
		return nullptr;
	}
	virtual void setCommonDataStorage(ComponentCommonData& commonData)
	{

	}

	virtual std::unique_ptr<ComponentType> clone() const = 0;

protected:
	OwnerType* m_owner;
};