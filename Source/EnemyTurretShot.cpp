#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/sphere.fbx"));

    // スケーリング
    //GetOwner()->GetTransform()->SetScaleFactor(0.015f);

    GetOwner()->GetTransform()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));

    // 位置設定
    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0.0f, 26.0f, 0.0f));

    // 速度設定
    DirectX::XMFLOAT3 moveVec{ 0.0f,0.0f,-8000.0f };
    SetVelocity(moveVec);
}

void EnemyTurretShot::Update()
{
    //DirectX::XMFLOAT3 pos = GetOwner()->GetTransform()->GetPosition();
    //pos.y = 26.0f;
    //GetOwner()->GetTransform()->SetPosition(pos);

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
