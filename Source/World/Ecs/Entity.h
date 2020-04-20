#pragma once

#include "Component.h"

#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <cstdint>

class Entity final
{
public:
	const std::uint32_t id;

	Entity(std::uint32_t id) noexcept;

	void addComponent(std::unique_ptr<Component> component) noexcept;
	template<typename T> void removeComponent() noexcept;

	template<typename T> T *getComponent() noexcept;
	template<typename T> const T *getComponent() const noexcept;

private:
	std::unordered_map<const std::type_info *, std::unique_ptr<Component>> components;
};

template<typename T> void Entity::removeComponent() noexcept
{
	RenderComponent *const rc = getComponent<RenderComponent>();
	CollisionComponent *const cc = getComponent<CollisionComponent>();

	if (rc)
		rc->compartmentalized = false;
	if (cc)
		cc->compartmentalized = false;

	components.erase(&typeid(T));
}

template<typename T> T *Entity::getComponent() noexcept
{
	if (components.count(&typeid(T)) != 0)
		return static_cast<T *>(components.at(&typeid(T)).get());

	return nullptr;
}

template<typename T> const T *Entity::getComponent() const noexcept
{
	if (components.count(&typeid(T)) != 0)
		return static_cast<const T *>(components.at(&typeid(T)).get());

	return nullptr;
}