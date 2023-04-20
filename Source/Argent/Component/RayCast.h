#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include "../Resource/Mesh.h"
#include "../Debug/DebugRenderer.h"
#include "../Math/MathHelper.h"


#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/unordered_map.hpp>


//todo raycastCollider�ɂ����������p�̏����͕K�v�H�@����onCollision�ł͂��߂ł����H
namespace Argent::Component
{
	//�O���錾
	namespace Collision
	{
		class RayCast;
	}
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
				Mesh,
				Max,
			};
			RayCastCollider(MeshType type = MeshType::Cylinder,
				const DirectX::XMFLOAT3& offset = DirectX::XMFLOAT3(),
					const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),
					const DirectX::XMFLOAT4& rotation = DirectX::XMFLOAT4(1, 1, 1, 1));

			RayCastCollider(const MeshResource& mResource);
			RayCastCollider(std::vector<MeshResource> mResource);

			~RayCastCollider() override = default;

			void Render() const override;
			void Initialize() override;

			DirectX::XMMATRIX GetWorldTransform();

			void DrawDebug() override;


			MeshType type;
			std::unique_ptr<Debug::DebugRenderer> debugRenderer;
			DirectX::XMFLOAT3 offset;
			DirectX::XMFLOAT3 scale;
			DirectX::XMFLOAT4 rotation;

			const MeshResource& GetMeshResource()const
			{
				return mResources[static_cast<int>(type)];
			}

			std::vector<MeshResource> GetMeshResourceVec() const
			{
				return mResource;
			}
		protected:
			std::vector<MeshResource> mResource;
			static MeshResource mResources[static_cast<int>(MeshType::Max)];
		};
		
	}

	namespace Collision
	{
		struct HitResult
		{
			DirectX::XMFLOAT3	position = {};		//���C�ƃ|���S���̌�_
			DirectX::XMFLOAT3	normal = {};		//�Փ˂����|���S���̖@���x�N�g��
			float				distance = 0.0f;	//���C�̎��_�����_�܂ł̋���
			int					materialIndex = -1;	//�Փ˂����|���S���̃}�e���A���ԍ�
		};


		class RayCast :
			public BaseComponent
		{
		public:
			RayCast();
				
			void SetRayData(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& direction, float length)
			{
				this->start = start;
				//this->direction = direction;
				//this->length = length;
				end = start + direction * length;
			}

			void SetRayData(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end)
			{
				this->start = start;
				this->end = end;
			}


			void SetRayStartPosition(const DirectX::XMFLOAT3& f) { start = f; }
			//void SetRayDirection(const DirectX::XMFLOAT3& f) { direction = f; }
			//void SetRayLength(float f) { length = f; }

			bool CollisionDetection(Collider::RayCastCollider* other, HitResult& hitResult) const;


			void DrawDebug() override;

		protected:
			DirectX::XMFLOAT3 start;
			/*DirectX::XMFLOAT3 direction;
			float length;*/
			DirectX::XMFLOAT3 end;
		};
	}

}