#include "EnemySpikeBot.h"
#include "EnemyStateDerivad.h"

EnemySpikeBot::EnemySpikeBot() : BaseEnemy("EnemySpikeBot")
{
}

void EnemySpikeBot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);
    
    // デバッグシリンダー
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cylinder));
    GetOwner()->GetComponent<Argent::Component::Collider::RayCastCollider>()->scale = { 500.0f,100.0f,500.0f };

    // ステートマシンをセット
    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new EnemyIdleState(this));
    stateMachine.get()->RegisterState(new EnemyAttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));

    // タグを設定する
    GetOwner()->ReplaceTag(GameObject::Tag::SpikeBot);
}

void EnemySpikeBot::Begin()
{
}

void EnemySpikeBot::Update()
{
    stateMachine.get()->Update();
}

void EnemySpikeBot::DrawDebug()
{
}
