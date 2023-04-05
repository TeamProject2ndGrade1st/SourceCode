#include "MrIncredible.h"
#include "Argent/GameObject/GameObject.h"
#include "Argent/Input/Keyboard.h"
#include "Argent/Core/Timer.h"



MrIncredible::MrIncredible():
	BaseActor("MrIncredible")
{
}

void MrIncredible::Initialize()
{
	ArComponent::Initialize();
	GetOwner()->GetTransform()->SetScaleFactor(0.01);
}

void MrIncredible::Update()
{
	const auto& keybord = Argent::Input::Keyboard::Instance();

	if(keybord.IsKeyPress(Argent::Input::Keyboard::W))
	{
		Transform* t = GetTransform();
		DirectX::XMFLOAT3 pos = t->GetPosition();
		pos.z += moveSpeed * Argent::Timer::ArTimer::Instance().DeltaTime();
		t->SetPosition(pos);
	}
	else if(keybord.IsKeyPress(Argent::Input::Keyboard::S))
	{
		Transform* t = GetTransform();
		DirectX::XMFLOAT3 pos = t->GetPosition();
		pos.z -= moveSpeed * Argent::Timer::ArTimer::Instance().DeltaTime();
		t->SetPosition(pos);
	}
}

void MrIncredible::OnCollision(const Argent::Component::Collider::Collider* collider)
{
	/*Transform* t = GetOwner()->GetTransform();
	DirectX::XMFLOAT3 pos = t->GetPosition();
	pos.x += 0.5f;
	t->SetPosition(pos);
	BaseActor::OnCollision(collider);*/
}

#ifdef _DEBUG
void MrIncredible::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		ImGui::SliderFloat("moveSpeed", &moveSpeed, 0.1f, 10.0f);
		BaseActor::DrawDebug();
		ImGui::TreePop();
	}
}
#endif