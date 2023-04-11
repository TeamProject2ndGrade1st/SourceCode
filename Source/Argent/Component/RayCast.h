#pragma once
#include "Component.h"
#include <memory>
#include <vector>

//todo raycastColliderにあたった時用の処理は必要？　両方onCollisionではだめですか？
namespace Argent::Component
{
	//前方宣言
	class RayCast;


	namespace Collider
	{
		class RayCastCollider:
			public BaseComponent
		{
		public:
			RayCastCollider():
				BaseComponent("RayCastCollider")
			{}

			~RayCastCollider() override = default;

			//std::vector<std::vector<Argent::Resource::Mesh::Vertex>> vertices;
			//std::vector<std::vector<uint32_t>> indices;
		};
	}


	class RayCast :
		public BaseComponent
	{
	public:
		RayCast():
			BaseComponent("RayCast")
		{}

		void SetRayStartPosition(const DirectX::XMFLOAT3& f) { start = f; }
		void SetRayDirection(const DirectX::XMFLOAT3& f) { direction = f; }
		void SetRayRange(float f) { range = f; }

		void OnCollision(Collider::RayCastCollider* other);

		void CollisionDetection(Collider::RayCastCollider* other);
	protected:
		DirectX::XMFLOAT3 start;
		DirectX::XMFLOAT3 direction;
		float range;
	};


}