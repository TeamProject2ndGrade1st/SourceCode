#include "Bullet.h"

Bullet::Bullet(const DirectX::XMFLOAT3& direction, 
	float damage, float speed):
	BaseActor("bullet")
,	damage(damage)
,	speed(speed)
,	direction(direction)
{
}

void Bullet::Initialize()
{
	
}

void Bullet::Update()
{
	Transform* t = GetTransform();
	DirectX::XMFLOAT3 pos = t->GetPosition();
	pos = direction * speed;
	t->AddPosition(pos);

	ray->SetRayData(pos, direction, speed);
	HitResult hitResult{};

	//todo レイキャストのコリジョン対象を絞れるようにする
	if(Argent::Collision::RayCollisionDetection(ray, hitResult))
	{
		GameObject::Destroy(GetOwner());
	}
}

void Bullet::Shot(Bullet* bulletActor, const Transform& t)
{
	const auto g = GameObject::Instantiate("bullet", bulletActor);
	g->GetTransform()->SetTransform(t);
}

void Bullet::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		ImGui::SliderFloat("Damage", &damage, 0, 10.0f);
		ImGui::SliderFloat("Speed", &speed, 0, 100.0f);
		ImGui::InputFloat3("Direction", &direction.x);
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}

