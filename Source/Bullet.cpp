#include "Bullet.h"

#include "BaseFriend.h"

Bullet::Bullet(const DirectX::XMFLOAT3& direction, 
               int damage, float speed,
	Mode mode):
	BaseActor("bullet")
,	damage(damage)
,	speed(speed)
,	direction(direction)
,	mode(mode)
{
	ray = new Argent::Component::Collision::RayCast();
}

void Bullet::Initialize()
{
	GetFriendManagerFromScene();

	//todo デバッグ表示用の何かを作ろうや
	/*auto* rC = new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Sphere);
	rC->scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	GetOwner()->AddComponent(rC);*/
	auto e = new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/barel_test2.efk", "./Resources/Effects");
	e->scale = { 5,5,5};
	GetOwner()->AddComponent(e);
	e->OnPlay(0);
}

void Bullet::Update()
{
	GetFriendManagerFromScene();
	Move();

	ManageDuration();
}

void Bullet::Shot(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& rotation,
	const DirectX::XMFLOAT3& direction, int damage, float speed,
	Mode mode)
{
	auto* b = new Bullet(direction, damage, speed, mode);
	const auto g = GameObject::Instantiate("Bullet", b);
	g->GetTransform()->SetPosition(position);
	g->GetTransform()->SetRotation(rotation);
}

void Bullet::ManageDuration()
{
	elapsedTime += Argent::Timer::GetDeltaTime();
	if (elapsedTime > validTime)
	{
		GameObject::Destroy(GetOwner());
	}
}

void Bullet::Move()
{
	const DirectX::XMFLOAT3 curPos = GetTransform()->GetPosition();
	float frameSpeed = speed * Argent::Timer::GetDeltaTime();
	const DirectX::XMFLOAT3 end = curPos + direction * frameSpeed;
	ray->SetRayData(curPos, end);
	HitResult hitResult{};
	if (Argent::Collision::RayCollisionDetection(ray, hitResult))
	{
		OnCollision(hitResult);
	}
	else
	{
		auto p = curPos + direction * frameSpeed;
		GetTransform()->SetPosition(p);
	}
}

void Bullet::OnCollision(const HitResult& result)
{
	//衝突判定をする相手のタグ
	const auto colliderTag = static_cast<unsigned>(GameObject::Tag::Friend);
	if(result.collider->GetOwner()->GetUnsignedTag() & colliderTag)
	{
		 if(friendManager)
		 {
			BaseFriend* friendCom = friendManager->FindFriendComponentFromOwner(result.collider->GetOwner());
			if (!(result.collider->GetOwner()->GetUnsignedTag() & static_cast<unsigned>(GameObject::Tag::Friend))) return;
			const GameObject* friendOwner = friendCom->GetOwner();

			//弾丸のモードによって処理を変える
	 		switch (mode)
			{
	 		case Mode::Creature:

				if(friendOwner->GetUnsignedTag() & static_cast<unsigned>(GameObject::Tag::Creature))
				{
					friendCom->ApplyHeal(damage);
				}
				else if(friendOwner->GetUnsignedTag() & static_cast<unsigned>(GameObject::Tag::Machine))
				{
					friendCom->ApplyDamage(damage);
				}

			 break;
			case Mode::Machine:

				if(friendOwner->GetUnsignedTag() & static_cast<unsigned>(GameObject::Tag::Creature))
				{
					friendCom->ApplyDamage(damage);
				}
				else if(friendOwner->GetUnsignedTag() & static_cast<unsigned>(GameObject::Tag::Machine))
				{
					friendCom->ApplyHeal(damage);
				}

			 break;
			}
		 }
	}
	GameObject::Destroy(GetOwner());
}

void Bullet::AddDamage(BaseFriend* f)
{
	auto tag = f->GetOwner()->GetTag();
	
}

void Bullet::GetFriendManagerFromScene()
{
	if(friendManager) return;

	std::vector<GameObject*> g;
	GameObject::FindByTag(GameObject::Tag::FriendManager, g);
	if(g.size() > 0)
	{
		friendManager = g.at(0)->GetComponent<FriendManager>();
	}
}


void Bullet::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		ImGui::SliderInt("Damage", &damage, 0, 10.0f);
		ImGui::SliderFloat("Speed", &speed, 0, 2000.0f);
		ImGui::InputFloat3("Direction", &direction.x);
		ImGui::SliderFloat("ElapsedTime", &elapsedTime, 0.0f, validTime);
		if(ray)
		{
			ray->DrawDebug();
		}
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}

