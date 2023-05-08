#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/sphere.fbx"));

    // スケーリング
    GetOwner()->GetTransform()->SetScaleFactor(0.015f);

    // 位置設定
    init_position.y = 27.0f;
    init_position.z -= 15.0f;
    GetOwner()->GetTransform()->SetPosition(init_position);


    // 速度設定
    DirectX::XMFLOAT3 moveVec{ 0.0f,0.0f,-8000.0f };
    SetVelocity(moveVec);

    // 弾が消えるまでの時間を設定する
    eraseTimer = 2.0f;
}

void EnemyTurretShot::Update()
{
    UpdateMove();   // 移動処理

    // 弾を消すカウントダウン
    eraseTimer -= Argent::Timer::GetDeltaTime();
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
