#include "Component.h"

Argent::Component::BaseComponent::BaseComponent(const BaseComponent&& com) noexcept
{
	this->name = com.name;
	this->owner = com.owner;
}
