#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/sphere.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(10.0f);

    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));

    // 速度設定
    DirectX::XMFLOAT3 moveVec{ 0.0f,0.0f,-100.0f };
    SetVelocity(moveVec);
}

void EnemyTurretShot::Update()
{
    UpdateMove();   // 移動処理
}

void EnemyTurretShot::DrawDebug()
{
}

// 移動処理
void EnemyTurretShot::UpdateMove()
{
    GetOwner()->GetTransform()->AddPosition(DirectX::XMFLOAT3(
        velocity.x * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor(),
        0.0f,
        velocity.z * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor()
    ));
}
