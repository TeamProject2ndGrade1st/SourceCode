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
	Transform* t = GetTransform();
	DirectX::XMFLOAT3 pos = t->GetPosition();
	/*pos = direction * speed * Argent::Timer::GetDeltaTime();
	t->AddPosition(pos);

	*/
	ray->SetRayData(pos, direction, speed);

	HitResult hitResult{};

	//todo レイキャストのコリジョン対象を絞れるようにする
	if(Argent::Collision::RayCollisionDetection(ray, hitResult))
	{
		GameObject::Destroy(GetOwner());
	}
	else
	{
		GetTransform()->SetPosition(pos + direction * speed);
	}

	elapsedTime += Argent::Timer::GetDeltaTime();
	if(elapsedTime > validTime)
	{
		GameObject::Destroy(GetOwner());
	}
}

void Bullet::Shot(const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT3& direction, int damage, float speed)
{
	auto* b = new Bullet(direction, damage, speed);
	const auto g = GameObject::Instantiate("Bullet", b);
	g->GetTransform()->SetPosition(position);
}

void Bullet::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
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

