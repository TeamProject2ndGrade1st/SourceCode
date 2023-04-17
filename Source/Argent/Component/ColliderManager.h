#pragma once
#include "Collider.h"
#include "RayCast.h"
#include <vector>


namespace Argent::Collider
{
	class ArColliderManager
	{
	public:

		void RegisterCollider(Argent::Component::Collider::Collider* c)
		{
			collider.emplace_back(c);
		}

		void RegisterRayCastCollider(Argent::Component::Collider::RayCastCollider* c)
		{
			rayCastCollider.emplace_back(c);
		}

		/*void RegisterRay(Argent::Component::Collision::RayCast* ray)
		{
			rayCast.emplace_back(ray);
		}*/

		void CollisionDetection();

		static ArColliderManager& Instance()
		{
			static ArColliderManager instance;
			return instance;
		}

		bool CollisionDetectionRayCast(Component::Collision::RayCast* ray, Component::Collision::HitResult& hitResult);
	private:
		std::vector<Component::Collider::Collider*> collider{};
		std::vector<Component::Collider::RayCastCollider*> rayCastCollider{};
		//std::vector<Component::Collision::RayCast*> rayCast{};
		ArColliderManager()
		{
			collider.clear();
		}
	};

	
}

