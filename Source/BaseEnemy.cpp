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
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));

    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new EnemyIdleState(this));
    stateMachine.get()->RegisterState(new EnemyAttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
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
