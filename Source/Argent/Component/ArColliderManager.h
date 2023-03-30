#pragma once
#include "Collider.h"
#include <vector>


namespace Argent::Collider
{
	class ArColliderManager
	{
	public:

		void AddCollider(Argent::Component::Collider::Collider* collider)
		{
			colliderLists.emplace_back(collider);
		}


		void CollisionDetection();

		static ArColliderManager& Instance()
		{
			static ArColliderManager instance;
			return instance;
		}


	private:
		std::vector<Component::Collider::Collider*> colliderLists{};
		ArColliderManager()
		{
			colliderLists.clear();
		}
	};

	
}

