#pragma once
#include "Component.h"
#include "Collider.h"
#include "Transform.h"


namespace Argent::Component
{
	class BaseActor:
		public BaseComponent
	{
	public:
		BaseActor(const char* name);

		virtual ~BaseActor() override = default;

		void Initialize() override;
		virtual void OnCollision(const Collider::Collider* collider) {}

	protected:
		Transform* GetTransform() const;
	};
	
}

