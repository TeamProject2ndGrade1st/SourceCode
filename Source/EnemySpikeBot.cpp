#include "EnemySpikeBot.h"
#include "EnemyStateDerivad.h"

void EnemySpikeBot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike.fbx"));
    

    // �X�P�[�����O
    GetOwner()->GetTransform()->SetScaleFactor(0.2f);   


    // �f�o�b�O�V�����_�[
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    GetOwner()->GetComponent<Argent::Component::Collider::RayCastCollider>()->scale = { 500.0f,100.0f,500.0f };
    
    // �G�t�F�N�g
    effect = new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/shield_bash.efk", "./Resources/Effects");
    GetOwner()->AddComponent(effect);
    effect->offset = { 0.0f,30.0f,-35.0f };
    effect->scale = { 7.0f,8.0f,7.0f };

    // �X�e�[�g�}�V�����Z�b�g
    stateMachine.reset(new EnemyStateMachine);

    stateMachine.get()->RegisterState(new Enemy::SpikeBot::IdleState(this));
    stateMachine.get()->RegisterState(new Enemy::SpikeBot::AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));

    // �^�O��ݒ肷��
    GetOwner()->ReplaceTag(GameObject::Tag::Enemy);

    SetAccelaration(0);
    BaseEnemy::Initialize();
}

void EnemySpikeBot::Begin()
{
    BaseEnemy::Begin();
}

void EnemySpikeBot::Update()
{
    stateMachine.get()->Update();
    UpdateVelocity();
    UpdateMove();
    BaseEnemy::Update();

    
}

void EnemySpikeBot::DrawDebug()
{
    BaseEnemy::DrawDebug();
}
