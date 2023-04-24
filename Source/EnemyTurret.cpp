#include "EnemyTurret.h"

EnemyTurret::EnemyTurret() : BaseEnemy("EnemyTurret")
{
}

void EnemyTurret::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/turrets/gunturret_0422_5.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);

    // ポジションをセット
    DirectX::XMFLOAT3 pos = { 25.0f,0.0f,0.0f };
    GetOwner()->GetTransform()->SetPosition(pos);    

    // タグを設定する
    GetOwner()->SetTag(GameObject::Tag::Turret);
}

void EnemyTurret::Begin()
{
}

void EnemyTurret::Update()
{
}

void EnemyTurret::DrawDebug()
{
}
