#pragma once
#include "Component.h"
#include "Collider.h"
#include "Transform.h"
#include "RayCast.h"


namespace Argent::Component
{
	class BaseActor:
		public BaseComponent
	{
	public:
		BaseActor(const char* name);

		virtual ~BaseActor() override = default;

		virtual void OnCollision(const Collider::Collider* collider) {}
		virtual void OnRayCollision(const Collider::RayCastCollider* other) {}

	protected:
		Transform* GetTransform() const;

	};
	
}

