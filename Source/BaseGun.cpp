#include "BaseGun.h"
#include "BaseBullet.h"


BaseGun::BaseGun():
	BaseActor("BaseGun")
{
		
}

void BaseGun::Shot()
{
	DirectX::XMFLOAT3 forward = GetTransform()->CalcForward();
	auto* bulletActor = new BaseBullet(forward, 0, 0.1f);

	Transform t = *(GameObject::FindGameObject("Camera")->GetTransform());
	t.SetScale(DirectX::XMFLOAT3(0.05f, 0.05f, 0.05f));
	BaseBullet::Shot(bulletActor, t);
}

void BaseGun::Initialize()
{
	GetOwner()->AddComponent(new BaseBullet(DirectX::XMFLOAT3(), 0, 0));
	BaseActor::Initialize();

}

void BaseGun::Update()
{
	if(Argent::Input::GetKeyDown(KeyCode::X))
	{
		Shot();
	}
	//Recoil();
}
