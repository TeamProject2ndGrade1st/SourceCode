#include "BaseGun.h"

BaseGun::BaseGun(const char* name):
	BaseComponent(name)
{
	 
}

void BaseGun::Initialize()
{
	lmg = new GameObject("LMG", 
		Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG/LMG_low_0324_1.fbx"));
	lmg->GetTransform()->SetPosition(DirectX::XMFLOAT3(2.0f, -1.5f, 2.0f));
	//g->GetTransform()->SetRotation(DirectX::XMFLOAT4(-9.0f, 80.0f, 0, 0));
	GetOwner()->AddChild(lmg);
}

void BaseGun::Update()
{
	auto t = GetOwner()->GetTransform();
	DirectX::XMFLOAT3 f = t->CalcForward() * offset.z;
	DirectX::XMFLOAT3 u = t->CalcUp() * offset.y;
	DirectX::XMFLOAT3 r = t->CalcRight() * offset.x;

	lmg->GetTransform()->SetPosition(f + u + r);

#ifdef _DEBUG
	if(Argent::Input::GetKeyUp(KeyCode::F3))
	{
		enableShot = !enableShot;
	}
#endif
	static float elapsedTime = 0;
	elapsedTime += Argent::Timer::GetDeltaTime();
	if(Argent::Input::GetButton(MouseButton::LeftButton))
	{
#ifdef _DEBUG
		if (!enableShot) return;
#endif
		if(elapsedTime > fireRate)
		{
			elapsedTime = 0;
			auto t = GetOwner()->GetTransform();
			Bullet::Shot(t->GetPosition(), t->CalcForward(),
				damage, speed, mode);
		}
	}

	if(Argent::Input::GetButtonUp(MouseButton::RightButton))
	{
		if(mode == Bullet::Mode::Living) mode = Bullet::Mode::Machine;
		if(mode == Bullet::Mode::Machine) mode = Bullet::Mode::Living;
	}

}

void BaseGun::Finalize()
{
	Bullet::ClearFriendManager();
}


void BaseGun::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		//Apex R-99ÇÃÉåÅ[ÉgÇ™ç≈í·íl
		ImGui::DragFloat("Fire Rate", &fireRate, 0.05f, 0.05f, 10.0f);
		ImGui::SliderInt("Damage", &damage, 0, 10);
		ImGui::SliderFloat("Speed", &speed, 1.0f, 100.0f);
		ImGui::DragFloat3("LMG Offset", &offset.x, 0.1f, -100, 100);
#ifdef _DEBUG
		ImGui::Checkbox("Enable Shot", &enableShot);
#endif
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
