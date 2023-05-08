#include "EnemyTurret.h"
#include "EnemyStateDerivad.h"

void EnemyTurret::Initialize()
{
    BaseEnemy::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/gun_turret_animv2.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.2f);

    // 向きを整える
    GetOwner()->GetTransform()->SetRotation(DirectX::XMFLOAT4(0.0f, 90.0f, 0.0f, 0.0f));

    // ステートマシンをセット
    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new Enemy::Turret::StartUpState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::AttackState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::IdleState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::BootWaitState(this));

    stateMachine.get()->SetState(static_cast<int>(State::BootWait));

    // タグを設定する
    GetOwner()->ReplaceTag(GameObject::Tag::Enemy);

    // ポジションをセット
    //DirectX::XMFLOAT3 pos = { 0.0f,0.0f,60.0f };
    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, 60.0f));

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
