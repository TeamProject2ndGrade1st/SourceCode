#include "ColliderManager.h"

#include "../GameObject/GameObject.h"

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

	bool ColliderManager::CollisionDetectionRayCast(Component::Collision::RayCast* ray, Component::Collision::HitResult& hitResult,
		unsigned int tag)
	{
		bool ret = false;
		for (size_t j = 0; j < rayCastCollider.size(); ++j)
		{
			const auto collider = rayCastCollider.at(j);

			//�Փ˔�����s���I�u�W�F�N�g�̃^�O���w�肳��Ă����ꍇ
			bool isSpecified = tag & static_cast<unsigned>(COLLISION_ALL_OBJECT);
			if(!isSpecified)
			{
				auto g = collider->GetOwner();
				if(!g) _ASSERT_EXPR(FALSE, L"return NULL Reference :: GetOwner()");

				//�w�肳�ꂽ�^�O�ƃI�u�W�F�N�g�̃^�O���Ⴄ�ꍇ�͏������΂�
				unsigned int tg = static_cast<unsigned>(g->GetTag());
				bool result = tag & tg;
				if(!result)	continue;
			}
			bool b = ray->CollisionDetection(collider, hitResult);
			if(b && !ret)
			{
				ret = true;
			}
		}
		return ret;
	}
}
