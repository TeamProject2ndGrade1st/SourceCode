#include "BaseBullet.h"

#include "Argent/GameObject/GameObject.h"
#include "Argent/Math/MathHelper.h"
#include "Argent/Component/Collider.h"
#include "Argent/Loader/FbxLoader.h"
#include "Argent/Input/Keyboard.h"

BaseBullet::BaseBullet(const DirectX::XMFLOAT3& direction, 
	float damage, float speed):
	BaseActor("bullet")
,	damage(damage)
,	speed(speed)
,	direction(direction)
{
}

void BaseBullet::OnCollision(const Argent::Component::Collider::Collider* collider)
{
}

void BaseBullet::Initialize()
{
	GetOwner()->AddComponent(new Argent::Component::Collider::SphereCollider);
	GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/cube.fbx"));
}

void BaseBullet::Update()
{
	Transform* t = GetTransform();
	DirectX::XMFLOAT3 pos = t->GetPosition();
	pos = direction * speed;
	t->AddPosition(pos);

	if(Argent::Input::Keyboard::Instance().IsKeyPress(Argent::Input::Keyboard::X))
	{
		GameObject::DestroyGameObject(GetOwner());
	}

}

#ifdef _DEBUG
void BaseBullet::DrawDebug()
{
	if(ImGui::Button("Destroy"))
	{
		GameObject::DestroyGameObject(GetOwner());
	}
	BaseActor::DrawDebug();
}
#endif
