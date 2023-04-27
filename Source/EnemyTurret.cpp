#include "EnemyTurret.h"

void EnemyTurret::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/turrets/gunturret_0422_5.fbx"));

    // �X�P�[�����O
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);

    // �|�W�V�������Z�b�g
    DirectX::XMFLOAT3 pos = { 25.0f,0.0f,0.0f };
    GetOwner()->GetTransform()->SetPosition(pos);    

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
