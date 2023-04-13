#include "ArColliderManager.h"

void Argent::Collider::ArColliderManager::CollisionDetection() const
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
