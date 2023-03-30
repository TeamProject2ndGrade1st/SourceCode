#include "ArColliderManager.h"

void Argent::Collider::ArColliderManager::CollisionDetection()
{
	for(size_t i = 0; i < colliderLists.size(); ++i)
	{
		for(size_t j = i + 1; j < colliderLists.size(); ++j)
		{
			auto c1 = colliderLists.at(i);
			auto c2 = colliderLists.at(j);
			if(c1->CollisionDetection(c2))
			{
				c1->OnCollision(c2);
				c2->OnCollision(c1);
			}
		}
	}
}
