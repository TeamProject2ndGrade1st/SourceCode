#pragma once
#include "Collider.h"
#include "RayCast.h"
#include <vector>


namespace Argent::Collider
{
	class ColliderManager
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


		void UnRegisterCollider(Argent::Component::Collider::Collider* c)
		{
			for(auto it = collider.begin(); it != collider.end(); ++it)
			{
				if((*it) == c)
				{
					it = collider.erase(it);
					break;
				}
			}
		}

		void UnRegisterRayCastCollider(Argent::Component::Collider::RayCastCollider* c)
		{
			for(auto it = rayCastCollider.begin(); it != rayCastCollider.end(); ++it)
			{
				if((*it) == c)
				{
					it = rayCastCollider.erase(it);
					break;
				}
			}
		}

		/*void RegisterRay(Argent::Component::Collision::RayCast* ray)
		{
			rayCast.emplace_back(ray);
		}*/

		void CollisionDetection();

		static ColliderManager& Instance()
		{
			static ColliderManager instance;
			return instance;
		}

		bool CollisionDetectionRayCast(Component::Collision::RayCast* ray, Component::Collision::HitResult& hitResult);
	private:
		std::vector<Component::Collider::Collider*> collider{};
		std::vector<Component::Collider::RayCastCollider*> rayCastCollider{};
		//std::vector<Component::Collision::RayCast*> rayCast{};
		ColliderManager()
		{
			collider.clear();
		}
	};

	
}

