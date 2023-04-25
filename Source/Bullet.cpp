#include "Bullet.h"

Bullet::Bullet(const DirectX::XMFLOAT3& direction, 
	int damage, float speed):
	BaseActor("bullet")
,	damage(damage)
,	speed(speed)
,	direction(direction)
{
	ray = new Argent::Component::Collision::RayCast();
}

void Bullet::Initialize()
{
	//todo デバッグ表示用の何かを作ろうや
	auto* rC = new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Sphere);
	rC->scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
	GetOwner()->AddComponent(rC);
}

void Bullet::Update()
{
	Move();

	ManageDuration();
}

void Bullet::Shot(const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT3& direction, int damage, float speed)
{
	auto* b = new Bullet(direction, damage, speed);
	const auto g = GameObject::Instantiate("Bullet", b);
	g->GetTransform()->SetPosition(position);
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
	ray->SetRayData(curPos, direction, speed);
	HitResult hitResult{};
	if (Argent::Collision::RayCollisionDetection(ray, hitResult))
	{
		OnCollision();
	}
	else
	{
		GetTransform()->SetPosition(curPos + direction * speed);
	}
}

void Bullet::OnCollision()
{
	GameObject::Destroy(GetOwner());
}

void Bullet::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		ImGui::SliderInt("Damage", &damage, 0, 10.0f);
		ImGui::SliderFloat("Speed", &speed, 0, 100.0f);
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

