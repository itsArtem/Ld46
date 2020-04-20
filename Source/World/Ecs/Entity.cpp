#include "Entity.h"

#include <utility>

Entity::Entity(std::uint32_t id) noexcept
	: id{id}
{
}

void Entity::addComponent(std::unique_ptr<Component> component) noexcept
{
	components[&typeid(*component)] = std::move(component);
}
