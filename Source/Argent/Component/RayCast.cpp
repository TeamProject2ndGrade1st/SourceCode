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

					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResources[static_cast<int>(MeshType::Cube)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Sphere.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

					std::ifstream ifs(filePath, std::ios::binary);
					cereal::BinaryInputArchive deserialization(ifs);
					deserialization(mResources[static_cast<int>(MeshType::Sphere)]);
				}
				{
					const char* filePath = "./Resources/Model/Collision/Cylinder.cereal";
					_ASSERT_EXPR(std::filesystem::exists(filePath), L"指定されたファイルが無い");

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

		RayCastCollider::RayCastCollider(std::vector<MeshResource> mResource):
			BaseComponent("RayCastCollider")
		,	offset(DirectX::XMFLOAT3())
		,	scale(DirectX::XMFLOAT3(1, 1, 1))
		,	rotation(DirectX::XMFLOAT4())
		,	type(MeshType::Mesh)
		{
			this->mResource = mResource;
			debugRenderer = std::make_unique<Debug::DebugRenderer>(
				mResource.at(0));

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
				auto t = *(GetOwner()->GetTransform());
				const auto p = t.GetPosition();
				t.SetPosition(offset + p);
				t.SetScale(t.GetScale() * scale);
				t.SetRotation(t.GetRotation() + rotation);
				debugRenderer->Render(t.GetWorld());
			}
		}

		void RayCastCollider::Initialize()
		{
			auto t = GetOwner()->GetTransform();
			t->SetPosition(DirectX::XMFLOAT3(0, 1, 3));
		}

		DirectX::XMMATRIX RayCastCollider::GetWorldTransform()
		{
			auto g = GetOwner();
			auto m = GetOwner()->GetTransform()->CalcWorldMatrix();
			const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) };const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
			const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(offset.x,offset.y, offset.z) };

			auto lm = S * R * T;
			return m * lm;
		}


		void RayCastCollider::DrawDebug()
		{
			if (ImGui::TreeNode(GetName().c_str()))
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

		bool RayCast::CollisionDetection(Collider::RayCastCollider* other, HitResult& hitResult) const
		{
			bool ret = false;
		//	DirectX::XMFLOAT3 end = start + direction * length;
			auto g = GetOwner();
			auto og = other->GetOwner();
			if(other->type != Collider::RayCastCollider::MeshType::Mesh)
			{
				if(Helper::Collision::IntersectRayVsModel(start, end, other->GetMeshResource(), 
					other->GetWorldTransform(), hitResult))
				{
					//壁までのベクトル
					DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
					DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
					DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

					//壁の法線
					DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hitResult.normal);

					//入射ベクトルを法線に射影
					DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

					//補正位置の計算　
					DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
					DirectX::XMFLOAT3 collectPosition{};
					DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

					HitResult hitResult2;
					if(!Helper::Collision::IntersectRayVsModel(hitResult.position, collectPosition, other->GetMeshResource(), 
						other->GetWorldTransform(), hitResult2))
					{
						hitResult.position = collectPosition;
					}
					else
					{
						hitResult.position = hitResult2.position;
					}
					/*
					GetOwner()->GetTransform()->SetPosition(p);
					auto actor = GetOwner()->GetActor();
					if(actor)
						actor->OnRayCollision(other);*/
					ret = true;
					
				}
			}
			else
			{
				auto m = other->GetMeshResourceVec();

				for(auto& mR : m)
				{
					if(Helper::Collision::IntersectRayVsModel(start, end, mR, 
						other->GetWorldTransform(), hitResult))
					{
						//壁までのベクトル
						DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
						DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
						DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

						//壁の法線
						DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hitResult.normal);

						//入射ベクトルを法線に射影
						DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

						//補正位置の計算　
						DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
						DirectX::XMFLOAT3 collectPosition{};
						DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

						HitResult hitResult2;
						if(!Helper::Collision::IntersectRayVsModel(hitResult.position, collectPosition, other->GetMeshResource(), 
							other->GetWorldTransform(), hitResult2))
						{
							hitResult.position = collectPosition;
						}
						else
						{
							hitResult.position = hitResult2.position;
						}
						/*
						GetOwner()->GetTransform()->SetPosition(p);
						auto actor = GetOwner()->GetActor();
						if(actor)
							actor->OnRayCollision(other);*/
						ret = true;
						
					}
				}

			}
			return ret;
		}

		void RayCast::DrawDebug()
		{
			if(ImGui::TreeNode(GetName().c_str()))
			{
				ImGui::InputFloat3("Start", &start.x);
				ImGui::InputFloat3("End", &end.x);
				ImGui::TreePop();
			}
		}

	}
}
