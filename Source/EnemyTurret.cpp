#include "EnemyTurret.h"
#include "EnemyStateDerivad.h"

void EnemyTurret::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/turrets/gun_turret_anm.fbx"));

    // �X�P�[�����O
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);

    // �|�W�V�������Z�b�g
    DirectX::XMFLOAT3 pos = { 25.0f,0.0f,0.0f };
    GetOwner()->GetTransform()->SetPosition(pos);    

    // �X�e�[�g�}�V�����Z�b�g
    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new Enemy::Turret::IdleState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::AttackState(this));
    stateMachine.get()->RegisterState(new Enemy::Turret::StartUpState(this));

    stateMachine.get()->SetState(static_cast<int>(State::StartUp));

    // �^�O��ݒ肷��
    GetOwner()->ReplaceTag(GameObject::Tag::Enemy);

    BaseEnemy::Initialize();
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
