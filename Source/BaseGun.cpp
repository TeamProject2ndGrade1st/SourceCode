#include "BaseGun.h"

BaseGun::BaseGun(const char* name):
	BaseComponent(name)
{
	 
}

void BaseGun::Update()
{
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
		if (!enableShot) return;
		if(elapsedTime > fireRate)
		{
			elapsedTime = 0;
			auto t = GetOwner()->GetTransform();
			Bullet::Shot(t->GetPosition(), t->CalcForward(),
				damage, speed);
		}
	}
}


void BaseGun::DrawDebug()
{
	if(ImGui::TreeNode(GetName().c_str()))
	{
		//Apex R-99のレートが最低値
		ImGui::SliderFloat("Fire Rate", &fireRate, 0.05f, 10.0f);
		ImGui::SliderInt("Damage", &damage, 0.1f, 10.0f);
		ImGui::SliderFloat("Speed", &speed, 1.0f, 100.0f);
#ifdef _DEBUG
		ImGui::Checkbox("Enable Shot", &enableShot);
#endif
		BaseComponent::DrawDebug();
		ImGui::TreePop();
	}
}
