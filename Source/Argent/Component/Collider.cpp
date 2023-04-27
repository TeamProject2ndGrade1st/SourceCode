#include "Collider.h"

#include "../GameObject/GameObject.h"
#include "BaseActor.h"
#include "../Other/Helper.h"
#include "ColliderManager.h"
#include "../Math/MathHelper.h"

namespace Argent::Component::Collider
{
	Collider::Collider(const char* name, ColliderType type):
		BaseComponent(name)
		,	type(type)
	{
		Argent::Collider::ColliderManager::Instance().RegisterCollider(this);
	}

	Collider::~Collider()
	{
		Argent::Collider::ColliderManager::Instance().UnRegisterCollider(this);
	}

	void Collider::OnCollision(const Collider* collider)
	{
		BaseActor* actor = GetOwner()->GetActor();
		if (actor) actor->OnCollision(collider);
	}

	bool SphereCollider::CollisionDetection(Collider* other)
	{
		//todo コライダーのタイプを球体以外もできるようにすること
		Transform* thisTransform = GetOwner()->GetTransform();
		Transform* otherTransform = other->GetOwner()->GetTransform();
		if(!thisTransform || !otherTransform) _ASSERT_EXPR(false, "Missing Transform Component");


		SphereCollider* collider = static_cast<SphereCollider*>(other);

		DirectX::XMFLOAT3 outPosition{};
		bool ret = Helper::Collision::IntersectSphereVsSphere(
		thisTransform->GetPosition(), this->radius, 
			otherTransform->GetPosition(), collider->radius, outPosition);

		if(ret)
		{
			otherTransform->SetPosition(outPosition);
		}

		return ret;
	}


	void SphereCollider::DrawDebug()
	{
		if(ImGui::TreeNode(GetName()))
		{
			ImGui::SliderFloat("Radius", &radius, 0.0, 10.0f);
			ImGui::TreePop();
		}
		Collider::DrawDebug();
	}

	void Capsule::DrawDebug()
	{
	}

}
