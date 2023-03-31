#pragma once
#include "ArComponent.h"
#include "Collider.h"
#include "Transform.h"


namespace Argent::Component
{
	class BaseActor:
		public ArComponent
	{
	public:
		BaseActor(const char* name);

		virtual ~BaseActor() override = default;

		virtual void OnCollision(const Collider::Collider* collider) {}

	protected:
		Transform* GetTransform() const;

	};
	
}

