#include "ArColliderManager.h"

void Argent::Collider::ArColliderManager::CollisionDetection()
{
	for(size_t i = 0; i < collider.size(); ++i)
	{
		for(size_t j = i + 1; j < collider.size(); ++j)
		{
			auto c1 = collider.at(i);
			auto c2 = collider.at(j);
			if(c1->CollisionDetection(c2))
			{
				c1->OnCollision(c2);
				c2->OnCollision(c1);
			}
		}
	}


}
