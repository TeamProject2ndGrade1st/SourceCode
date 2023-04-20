#include "Demo.h"
#include "Argent/Component/ColliderManager.h"
#include "Argent/Input/Mouse.h"

void RayCastDemo::Initialize()
{
	ray = new Argent::Component::Collision::RayCast();
	GetOwner()->AddComponent(ray);
	BaseActor::Initialize();
	moveSpeed = 1.0f;
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver8.fbx", false));
	audio = new Argent::Component::Audio::AudioPlayer("./Resources/Audio/maou.wav");
	GetOwner()->AddComponent(audio);
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
		ray->SetRayData(GetTransform()->GetPosition(), GetTransform()->CalcForward() * -1, moveSpeed * Argent::Timer::GetDeltaTime());
		HitResult hitResult;
		if (Argent::Collider::ArColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult))
		{
			GetTransform()->SetPosition(hitResult.position);
		}
		else
		{
			const auto p = GetTransform()->GetPosition() + GetTransform()->CalcForward() * -1  * moveSpeed * Argent::Timer::GetDeltaTime();
			GetTransform()->SetPosition(p);
		}
	}
	if (Argent::Input::GetKey(KeyCode::J))
	{
		ray->SetRayData(GetTransform()->GetPosition(), GetTransform()->CalcRight() * -1, moveSpeed * Argent::Timer::GetDeltaTime());
		HitResult hitResult;
		if (Argent::Collider::ArColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult))
		{
			GetTransform()->SetPosition(hitResult.position);
		}
		else
		{
			const auto p = GetTransform()->GetPosition() + GetTransform()->CalcRight() * -1  * moveSpeed * Argent::Timer::GetDeltaTime();
			GetTransform()->SetPosition(p);
		}
	}
	if (Argent::Input::GetKey(KeyCode::L))
	{
		auto r = GetTransform()->CalcRight();
		ray->SetRayData(GetTransform()->GetPosition(), GetTransform()->CalcRight(), moveSpeed * Argent::Timer::GetDeltaTime());
		HitResult hitResult;
		if (Argent::Collider::ArColliderManager::Instance().CollisionDetectionRayCast(ray, hitResult))
		{
			GetTransform()->SetPosition(hitResult.position);
		}
		else
		{
			const auto p = GetTransform()->GetPosition() + GetTransform()->CalcRight() * moveSpeed * Argent::Timer::GetDeltaTime();
			GetTransform()->SetPosition(p);
		}
	}
}

void RayCastDemo::DrawDebug()
{
	if (ImGui::TreeNode(GetName().c_str()))
	{
		mousePos = Argent::Input::Mouse::Instance().GetPosition();
		mousePostPos = Argent::Input::Mouse::Instance().GetPostPosition();
		auto vec = Argent::Input::Mouse::Instance().GetMoveVec();
		ImGui::InputFloat2("mouse", &mousePos.x);
		ImGui::InputFloat2("mousePost", &mousePostPos.x);
		ImGui::InputFloat2("mouseMoveVec", &vec.x);

		ImGui::SliderFloat("moveSpeed", &moveSpeed, 0, 10.0f);
		if(ImGui::Button("Destroy"))
		{
			GameObject::DestroyGameObject(GetOwner());
		}
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}
