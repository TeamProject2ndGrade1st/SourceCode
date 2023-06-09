#include "EnemyTurret.h"
#include "EnemyStateDerivad.h"

void EnemyTurret::Initialize()
{
    BaseEnemy::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/gun_turret_animv4.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.2f);

    // 向きを整える
    //GetOwner()->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, 90.0f, 0.0f, 0.0f));
    //GetOwner()->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

    // デバックシリンダー
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    GetOwner()->GetComponent<Argent::Component::Collider::RayCastCollider>()->scale = { 500.0f,100.0f,500.0f };


    // ステートマシンをセット
    //stateMachine.reset(new EnemyStateMachine);
    stateMachine = std::make_unique<EnemyStateMachine>();

    stateMachine.get()->RegisterState(new Enemy::Turret::StartUpState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::AttackState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::IdleState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::BootWaitState(this));

    stateMachine.get()->SetState(static_cast<int>(State::BootWait));

    // タグを設定する
    GetOwner()->ReplaceTag(GameObject::Tag::Enemy);

    // ポジションをセット
    //DirectX::XMFLOAT3 pos = { 0.0f,0.0f,60.0f };
    auto transform = GetOwner()->GetTransform();
}

void EnemyTurret::Begin()
{
    BaseEnemy::Begin();
}

void EnemyTurret::Update()
{
    BaseEnemy::Update();
}

void EnemyTurret::DrawDebug()
{
    BaseEnemy::DrawDebug();
}
