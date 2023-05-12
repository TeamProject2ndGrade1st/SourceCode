#include "BaseGun.h"

BaseGun::BaseGun(const char* name):
	BaseComponent(name)
{
	damage = 1;
	speed = 100.0f;
	fireRate = 1.0f;
}

void BaseGun::Initialize()
{
	lmg = new GameObject("LMG", 
		Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG/LMG_0406_ver4.fbx"));
	lmg->GetTransform()->SetPosition(DirectX::XMFLOAT3(-3.0f, -3.0f, 3.0f));
	lmg->GetTransform()->SetRotation(DirectX::XMFLOAT4(-2.0f, -86.5f, 0.0f, 0.0f));
	lmg->GetTransform()->SetScaleFactor(0.1f);
	GetOwner()->AddChild(lmg);
}

void BaseGun::Update()
{
	RecoilUpdate();
	TremorUpdate();
	
	auto t = GetOwner()->GetTransform();
	DirectX::XMFLOAT3 f = t->CalcForward() * (offset.z + tremorMove.z + recoilMove.z);
	DirectX::XMFLOAT3 u = t->CalcUp() * (offset.y + tremorMove.y + recoilMove.y);
	DirectX::XMFLOAT3 r = t->CalcRight() * (offset.x + tremorMove.x + recoilMove.x);

	t->SetPosition(t->GetPosition() + f + u + r);

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
			Bullet::Shot(t->GetPosition(), t->GetRotation(), t->CalcForward(),
				damage, speed, mode);
			AddRecoil(DirectX::XMFLOAT3(0, 0, -3),0.05f);
		}
	}

	if(Argent::Input::GetButtonUp(MouseButton::RightButton))
	{
		if(mode == Bullet::Mode::Creature) mode = Bullet::Mode::Machine;
		else if(mode == Bullet::Mode::Machine) mode = Bullet::Mode::Creature;
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
		//Apex R-99のレートが最低値
		ImGui::DragFloat("Fire Rate", &fireRate, 0.05f, 0.05f, 10.0f);
		ImGui::SliderInt("Damage", &damage, 0, 10);
		ImGui::SliderFloat("Speed", &speed, 1.0f, 100.0f);
		ImGui::DragFloat3("LMG Offset", &offset.x, 0.1f, -100, 100);

		ImGui::SliderFloat("TremorSpeed", &tremorSpeed, 0, 1000);
		ImGui::SliderFloat("TremorAmp", &tremorAmp, 0.01f, 1.00f);

		ImGui::SliderFloat("RecoilUpTime", &recoilUpTime, 0, 0.1f);
		ImGui::SliderFloat("RecoilDowmTime", &recoilDownTime, 0, 0.1f);
#ifdef _DEBUG
		ImGui::Checkbox("Enable Shot", &enableShot);
#endif
		BaseComponent::DrawDebug();
		GetOwner()->GetTransform()->DrawDebug();
		ImGui::TreePop();
	}
}

void BaseGun::RecoilUpdate()
{
	if (recoilPower.x || recoilPower.y || recoilPower.z)
	{
		if (recoilUpTimer <= 0)recoilUpTimer = recoilUpTime;

		float deltaTime = Argent::Timer::ArTimer::Instance().DeltaTime();

		recoilMove.x += recoilPower.x * (deltaTime / recoilUpTime);
		recoilMove.y += recoilPower.y * (deltaTime / recoilUpTime);
		recoilMove.z += recoilPower.z * (deltaTime / recoilUpTime);
		recoilUpTimer -= deltaTime;

		if (recoilUpTimer <= 0)
		{
			recoilPower = {};
		}

		//リコイル増加中は減衰処理まえにretuan
		return;
	}

	if (!(recoilMove.x || recoilMove.y || recoilMove.z))return;
	if (recoilDownTimer >= 0)
	{
		recoilDownTimer = recoilDownTime;
	}
	float deltaTime = Argent::Timer::ArTimer::Instance().DeltaTime();
	recoilMove.x -= recoilMove.x * (deltaTime/recoilDownTime);
	recoilMove.y -= recoilMove.y * (deltaTime/recoilDownTime);
	recoilMove.z -= recoilMove.z * (deltaTime/recoilDownTime);
	recoilDownTimer -= deltaTime;

	if (recoilDownTimer <= 0)
	{
		recoilDownTimer = 0;
		recoilMove = {};
	}
	
}

void BaseGun::TremorUpdate()
{
	//歩いてる時の揺れ
	if (walking)
	{
		tremorWalk += Argent::Timer::ArTimer::Instance().DeltaTime();
		float t = sinf(DirectX::XMConvertToRadians(tremorWalk) * tremorSpeed);
		tremorMove.y = t * tremorAmp;
	}
	else if (-0.01f > tremorMove.y || tremorMove.y > 0.01f)
	{
		tremorWalk += Argent::Timer::ArTimer::Instance().DeltaTime();
		float t = sinf(DirectX::XMConvertToRadians(tremorWalk) * tremorSpeed);
		tremorMove.y = t * tremorAmp;
	}
	else
	{
		tremorMove.y = 0;
		tremorWalk = 0;
	}
}

void BaseGun::AddRecoil(DirectX::XMFLOAT3 power,float downTime)
{
	recoilPower.x = power.x;
	recoilPower.y = power.y;
	recoilPower.z = power.z;

	recoilDownTime = downTime;
}
