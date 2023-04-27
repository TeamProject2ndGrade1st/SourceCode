#include "RayCast.h"
#include "../Math/MathHelper.h"
#include "../GameObject/GameObject.h"
#include "ColliderManager.h"

//#include "../Other/Helper.h"
//
namespace Argent::Component
{
	namespace Collider
	{
		Resource::Mesh::MeshResource RayCastCollider::mResources[] {};

		RayCastCollider::RayCastCollider(MeshType type,
			const DirectX::XMFLOAT3& offset, const DirectX::XMFLOAT3& scale,
			const DirectX::XMFLOAT4& rotation):
			BaseComponent("RayCastCollider")
		,	type(type)
		,	offset(offset)
		,	scale(scale)
		,	rotation(rotation)
		{
			static bool b = false;
			if(!b)
			{
				{
					const char* filePath = "./Resources/Model/Collision/Cube.cereal";

					_ASSERT_EXPR(std::filesystem::exists(filePath), L"�w�肳�ꂽ�t�@�C��������");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResources[static_cast<int>(MeshType::Cube)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Sphere.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"�w�肳�ꂽ�t�@�C��������");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResources[static_cast<int>(MeshType::Sphere)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Cylinder.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"�w�肳�ꂽ�t�@�C��������");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResources[static_cast<int>(MeshType::Cylinder)]);
				}
				b = true;
			}
			debugRenderer = std::make_unique<Debug::DebugRenderer>(
				mResources[static_cast<int>(type)]);

			Argent::Collider::ColliderManager::Instance().RegisterRayCastCollider(this);
		}

		RayCastCollider::RayCastCollider(const MeshResource& mResource):
			BaseComponent("RayCastCollider")
		,	offset(DirectX::XMFLOAT3())
		,	scale(DirectX::XMFLOAT3(1, 1, 1))
		,	rotation(DirectX::XMFLOAT4())
		,	type(MeshType::Mesh)
		{
			this->mResource.emplace_back(mResource);
			debugRenderer = std::make_unique<Debug::DebugRenderer>(
				mResource);

			Argent::Collider::ColliderManager::Instance().RegisterRayCastCollider(this);
		}

		RayCastCollider::RayCastCollider(std::vector<MeshResource> meshResource):
			BaseComponent("RayCastCollider")
		,	offset(DirectX::XMFLOAT3())
		,	scale(DirectX::XMFLOAT3(1, 1, 1))
		,	rotation(DirectX::XMFLOAT4())
		,	type(MeshType::Mesh)
		{
			this->mResource = meshResource;
			if(meshResource.size() > 0)
			{
				debugRenderer = std::make_unique<Debug::DebugRenderer>(
					mResource.at(0));
			}

			Argent::Collider::ColliderManager::Instance().RegisterRayCastCollider(this);

		}

		RayCastCollider::~RayCastCollider()
		{
			Argent::Collider::ColliderManager::Instance().UnRegisterRayCastCollider(this);
		}

		void RayCastCollider::Render() const
		{
			if(debugRenderer)
			{
				DirectX::XMFLOAT4X4 w{};
				DirectX::XMStoreFloat4x4(&w, GetWorldTransform());
				debugRenderer->Render(w);
			}
		}

		void RayCastCollider::Initialize()
		{
		}

		DirectX::XMMATRIX RayCastCollider::GetWorldTransform() const
		{
			auto g = GetOwner();
			/*auto t = g->GetTransform()->AdjustParentTransform();*/
			//auto m = t.CalcWorldMatrix();
			auto m = g->GetTransform()->CalcWorldMatrix();
			const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };
			const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
			const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(offset.x,offset.y, offset.z) };

			auto lm = S * R * T;
			return m * lm;
		}


		void RayCastCollider::DrawDebug()
		{
			if (ImGui::TreeNode(GetName()))
			{
				ImGui::DragFloat3("Offset", &offset.x);
				ImGui::DragFloat3("Scale", &scale.x);
				BaseComponent::DrawDebug();
				ImGui::TreePop();
			}
		}


	}

	namespace Collision
	{
		RayCast::RayCast():
			BaseComponent("RayCast")
		{
			//Argent::Collider::ColliderManager::Instance().RegisterRay(this);
		}

		void CalcComplementPosition(Collider::RayCastCollider* other,
			const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
			HitResult& hitResult)
		{
			//�ǂ܂ł̃x�N�g��
			const DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			const DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			const DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			//�ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hitResult.normal);

			//���˃x�N�g����@���Ɏˉe
			const DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			//�␳�ʒu�̌v�Z�@
			const DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition{};
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			//�ǃY���ʒu���v�Z
			HitResult hitResult2;
			if (!Helper::Collision::IntersectRayVsModel(hitResult.position, collectPosition, other->GetMeshResource(),
				other->GetWorldTransform(), hitResult2))
			{
				hitResult.position = collectPosition;
			}
			else
			{
				hitResult.position = hitResult2.position;
			}
		}

		bool RayCast::CollisionDetection(Collider::RayCastCollider* other, HitResult& hitResult) const
		{
			bool ret = false;
			if(other->type != Collider::RayCastCollider::MeshType::Mesh)
			{
				//todo�@�U���v�Z�@�R���C�_�[�̈ʒu�����̍��W����傫�����炳��Ă����ꍇ�͔��肪����Ȃ��悤�ɂȂ�
				if(GetOwner())
				{
					auto* t = GetOwner()->GetTransform();
					auto* t2 = other->GetOwner()->GetTransform();

					if(t && t2)
					{
						const float length = Length(t->GetPosition(), t2->GetPosition());
						const float rayLength = Length(start, end);
						//���ۂ̋����ƃ��C�̋������ׁ@���C�̂ق����Z�������ꍇ�͓�����Ȃ��͂��Ȃ̂Œʂ�Ȃ�
						//todo ���C�̃X�^�[�g�n�_���I�[�i�[�̍��W�Ƒ傫������Ă����ꍇ�͂��܂������Ȃ�
						if(length > rayLength) return false;
					}
					if(Helper::Collision::IntersectRayVsModel(start, end, other->GetMeshResource(), 
						other->GetWorldTransform(), hitResult))
					{
						hitResult.collider = other;
						CalcComplementPosition(other, start, end, hitResult);
						ret = true;
					}
				}
			}
			else
			{
				for(auto& m : other->GetMeshResourceVec())
				{
					if(Helper::Collision::IntersectRayVsModel(start, end, m, 
						other->GetWorldTransform(), hitResult))
					{
						hitResult.collider = other;

						CalcComplementPosition(other, start, end, hitResult);
						ret = true;
					}
				}

			}
			return ret;
		}

		void RayCast::DrawDebug()
		{
			if(ImGui::TreeNode(GetName()))
			{
				ImGui::InputFloat3("Start", &start.x);
				ImGui::InputFloat3("End", &end.x);
				ImGui::TreePop();
			}
		}

	}
}
