#include "ColliderManager.h"

namespace Argent::Collider
{
	void ColliderManager::CollisionDetection()
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
	}

	bool ColliderManager::CollisionDetectionRayCast(Component::Collision::RayCast* ray, Component::Collision::HitResult& hitResult)
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
