#include "RayCastDemo.h"
#include "Argent/Component/ArColliderManager.h"

void RayCastDemo::Initialize()
{
		ray = new Argent::Component::Collision::RayCast();
		GetOwner()->AddComponent(ray);
		BaseActor::Initialize();
		moveSpeed = 1.0f;
		GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false));
}

void RayCastDemo::Update()
{
	BaseActor::Update();

	if (Argent::Input::GetKey(KeyCode::I))
	{
		ray->SetRayData(GetTransform()->GetPosition(), GetTransform()->CalcForward(), moveSpeed * Argent::Timer::GetDeltaTime());
		HitResult hitResult;
		if(Argent::Collider::ArColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult))
		{
			GetTransform()->SetPosition(hitResult.position);
		}
		else
		{
			const auto p = GetTransform()->GetPosition() + GetTransform()->CalcForward() * moveSpeed * Argent::Timer::GetDeltaTime();
			GetTransform()->SetPosition(p);
		}
	}
	if (Argent::Input::GetKey(KeyCode::K))
	{
		ray->SetRayData(GetTransform()->GetPosition(), GetTransform()->CalcForward(), moveSpeed * Argent::Timer::GetDeltaTime());
		HitResult hitResult;
		if (Argent::Collider::ArColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult))
		{
			GetTransform()->SetPosition(hitResult.position);
		}
		else
		{
			const auto p = GetTransform()->GetPosition() * -moveSpeed * Argent::Timer::GetDeltaTime();
			GetTransform()->SetPosition(p);
		}
	}
}
#ifdef _DEBUG
void RayCastDemo::DrawDebug()
{
	if (ImGui::TreeNode(GetName().c_str()))
	{
		ImGui::SliderFloat("moveSpeed", &moveSpeed, 0, 10.0f);
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}
#endif