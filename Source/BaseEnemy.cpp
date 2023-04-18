#include "BaseEnemy.h"
#include "Argent/Argent.h"

BaseEnemy::BaseEnemy():
    BaseActor("BaseEnemy")
{
}

BaseEnemy::BaseEnemy(DirectX::XMFLOAT3 setPos)
    : BaseActor("BaseEnemy")
{
    GetOwner()->GetTransform()->SetPosition(setPos);
}

void BaseEnemy::Initialize()
{
    static bool flag;
    if (flag)return;
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    BaseActor::Initialize();
    flag = true;
}

void BaseEnemy::Begin()
{

}

void BaseEnemy::Update()
{
    Transform* transform = GetOwner()->GetTransform();
    //transform->AddPosition(speed);
}

void BaseEnemy::DrawDebug()
{
}
