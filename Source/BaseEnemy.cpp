#include "BaseEnemy.h"
#include "EnemyStateDerivad.h"

// コンストラクタ
BaseEnemy::BaseEnemy():Character("BaseEnemy")
{
}

BaseEnemy::BaseEnemy(DirectX::XMFLOAT3 pos)
    :Character("BaseEnemy")
{
    GetOwner()->GetTransform()->SetPosition(pos);
}

BaseEnemy::BaseEnemy(const char* name):Character(name)
{
}


void BaseEnemy::Initialize()
{
    

    //GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    
    
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = {
    //    attackAreaRadius 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f
    //};
    


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
