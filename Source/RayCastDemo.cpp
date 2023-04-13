#include "RayCastDemo.h"

void RayCastDemo::Initialize()
{
	static bool b = false;
	if (!b)
	{
		ray = new Argent::Component::Collision::RayCast();
		GetOwner()->AddComponent(ray);
		BaseActor::Initialize();
		moveSpeed = 1.0f;
		GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false));
		b = true;
	}
}

void RayCastDemo::OnRayCollision(const Argent::Component::Collider::RayCastCollider* other)
{
	BaseActor::OnRayCollision(other);
}

void RayCastDemo::Update()
{
	BaseActor::Update();

	if (Argent::Input::GetKey(KeyCode::I))
	{
		auto p = GetTransform()->GetPosition();
		p = p + GetTransform()->CalcForward() * moveSpeed * Argent::Timer::GetDeltaTime();
		GetTransform()->SetPosition(p);
	}
	if (Argent::Input::GetKey(KeyCode::K))
	{
		auto p = GetTransform()->GetPosition();
		p = p + GetTransform()->CalcForward() * -moveSpeed * Argent::Timer::GetDeltaTime();
		GetTransform()->SetPosition(p);
	}

	ray->SetRayStartPosition(GetTransform()->GetPosition());
	ray->SetRayDirection(GetTransform()->CalcForward());
	ray->SetRayLength(moveSpeed * Argent::Timer::GetDeltaTime());
}

void RayCastDemo::DrawDebug()
{
	if (ImGui::TreeNode(GetName().c_str()))
	{
		ImGui::SliderFloat("moveSpeed", &moveSpeed, 0, 10.0f);
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}
