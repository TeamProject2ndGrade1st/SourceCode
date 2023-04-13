#include "ArColliderManager.h"


namespace Argent::Collider
{
	void ArColliderManager::CollisionDetection()
	{
		for(size_t i = 0; i < collider.size(); ++i)
		{
			for(size_t j = i + 1; j < collider.size(); ++j)
			{
				const auto c1 = collider.at(i);
				const auto c2 = collider.at(j);
				if(c1->CollisionDetection(c2))
				{
					c1->OnCollision(c2);
					c2->OnCollision(c1);
				}
			}
		}

		/*for(size_t i = 0; i < rayCast.size(); ++i)
		{
			for(size_t j = 0; j < rayCastCollider.size(); ++j)
			{
				Argent::Component::Collision::RayCast* ray = rayCast.at(i);
				auto collider = rayCastCollider.at(j);
				ray->CollisionDetection(collider);
			}
		}*/
	}

	bool ArColliderManager::CollisionDetectionRayCast(Component::Collision::RayCast* ray, Component::Collision::HitResult& hitResult)
	{
		bool ret = false;
		for (size_t j = 0; j < rayCastCollider.size(); ++j)
		{
			const auto collider = rayCastCollider.at(j);
			bool b = ray->CollisionDetection(collider, hitResult);
			if(b && !ret)
			{
				ret = true;
			}
		}
		return ret;
	}
}
