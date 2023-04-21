#include "BaseEnemy.h"

// コンストラクタ
BaseEnemy::BaseEnemy():
    Character("BaseEnemy")
{
}


void BaseEnemy::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike_bot_0419_1.fbx"));
    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));


}

void BaseEnemy::Begin()
{
}

void BaseEnemy::Update()
{
}

void BaseEnemy::DrawDebug()
{
}
