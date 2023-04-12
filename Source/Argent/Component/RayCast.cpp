#include "RayCast.h"
#include "../Math/MathHelper.h"
#include "../GameObject/GameObject.h"
#include "ArColliderManager.h"

//#include "../Other/Helper.h"
//
namespace Argent::Component
{
	namespace Collider
	{
		Resource::Mesh::MeshResource RayCastCollider::mResource[] {};

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

					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResource[static_cast<int>(MeshType::Cube)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Sphere.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResource[static_cast<int>(MeshType::Sphere)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Cylinder.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResource[static_cast<int>(MeshType::Cylinder)]);
				}
				b = true;
			}
			debugRenderer = std::make_unique<Debug::DebugRenderer>(
				mResource[static_cast<int>(type)]);

			Argent::Collider::ArColliderManager::Instance().RegisterRayCastCollider(this);
		}

		
		void RayCastCollider::Render() const
		{
			if(debugRenderer)
			{
				auto t = *(GetOwner()->GetTransform());
				/*const auto p = t.GetPosition();
				t.SetPosition(offset + p);
				t.SetScale(t.GetScale() * scale);
				t.SetRotation(t.GetRotation() + rotation);*/
				debugRenderer->Render(t.GetWorld());
			}
		}

		void RayCastCollider::Initialize()
		{
			auto t = GetOwner()->GetTransform();
			t->SetPosition(DirectX::XMFLOAT3(0, 1, 3));
		}
	}

	namespace Collision
	{
		RayCast::RayCast():
			BaseComponent("RayCast")
		{
			Argent::Collider::ArColliderManager::Instance().RegisterRay(this);
		}

		void RayCast::OnCollision(Collider::RayCastCollider* other)
		{
			GameObject* obj = GetOwner();
			if(obj)
			{
				obj->GetActor()->OnRayCollision(other);
			}
		}

		void RayCast::CollisionDetection(Collider::RayCastCollider* other) const 
		{
			DirectX::XMFLOAT3 end = start + direction * length;
			HitResult hitResult;
			if(Helper::Collision::IntersectRayVsModel(start, end, other->GetMeshResource(), 
				other->GetOwner()->GetTransform()->GetWorldMatrix(), hitResult))
			{
				GetOwner()->GetTransform()->SetPosition(hitResult.position);
				auto actor = GetOwner()->GetActor();
				if(actor)
					actor->OnRayCollision(other);
			}
		}

		void RayCast::DrawDebug()
		{
			if(ImGui::TreeNode(GetName().c_str()))
			{
				ImGui::InputFloat3("Start", &start.x);
				ImGui::InputFloat3("Direction", &direction.x);
				ImGui::InputFloat("Length", &length);
				ImGui::TreePop();
			}
		}
	}
}
