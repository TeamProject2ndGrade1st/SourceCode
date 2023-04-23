#include "BaseEnemy.h"
#include "EnemyStateDerivad.h"

// コンストラクタ
BaseEnemy::BaseEnemy():
    Character("BaseEnemy")
{
}

BaseEnemy::BaseEnemy(DirectX::XMFLOAT3 pos)
    :Character("BaseEnemy")
{
    GetOwner()->GetTransform()->SetPosition(pos);
}


void BaseEnemy::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike.fbx"));
    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);
    //GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cylinder));
    
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = {
    //    attackAreaRadius 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f
    //};
    GetOwner()->GetComponent<Argent::Component::Collider::RayCastCollider>()->scale = { 500.0f,100.0f,500.0f };

    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new EnemyIdleState(this));
    stateMachine.get()->RegisterState(new EnemyAttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));

    // タグを設定する
    GetOwner()->SetTag(GameObject::Tag::SpikeBot);
}

void BaseEnemy::Begin()
{
}

void BaseEnemy::Update()
{
    stateMachine.get()->Update();

    
}

void BaseEnemy::DrawDebug()
{
}
