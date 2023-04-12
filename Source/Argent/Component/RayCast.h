#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include "../Resource/Mesh.h"
#include "../Debug/DebugRenderer.h"


#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>


//todo raycastColliderにあたった時用の処理は必要？　両方onCollisionではだめですか？
namespace Argent::Component
{
	//前方宣言
	class RayCast;
	using MeshResource = Resource::Mesh::MeshResource;

	namespace Collider
	{
		class RayCastCollider:
			public BaseComponent
		{
		public:
			enum class MeshType
			{
				Cube,
				Sphere,
				Cylinder,
				Max,
			};
			RayCastCollider(MeshType type = MeshType::Cylinder,
				const DirectX::XMFLOAT3& offset = DirectX::XMFLOAT3(),
					const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),
					const DirectX::XMFLOAT4& rotation = DirectX::XMFLOAT4(1, 1, 1, 1));

			~RayCastCollider() override = default;

			void Render() const override;

			MeshType type;
			std::unique_ptr<Debug::DebugRenderer> debugRenderer;
			DirectX::XMFLOAT3 offset;
			DirectX::XMFLOAT3 scale;
			DirectX::XMFLOAT4 rotation;

		protected:
			//MeshResource mResource;
			static MeshResource mResource[static_cast<int>(MeshType::Max)];
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