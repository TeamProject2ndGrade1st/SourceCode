#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/bullet_0509_1.fbx"));
    //GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/sphere.fbx"));

    // スケーリング
    //GetOwner()->GetTransform()->SetScaleFactor(0.015f);

    // 位置設定
    init_position.y = 27.0f;
    init_position.z -= 15.0f;
    GetOwner()->GetTransform()->SetPosition(init_position);


    // 速度設定
    DirectX::XMFLOAT3 velo = GetVelocity();
    float add = 100.0f;
    moveVec = { velo.x * add,velo.y * add ,velo.z * add };
    //DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&velo);
    //DirectX::XMVectorScale(vec, 1000.0f);
    //DirectX::XMStoreFloat3(&moveVec, vec);
    SetVelocity(moveVec);


#if 0
    // test
    DirectX::XMVECTOR myVec = DirectX::XMLoadFloat3(&velo);
    DirectX::XMFLOAT3 forward = GetOwner()->GetTransform()->CalcForward();
    DirectX::XMVECTOR fVec = DirectX::XMLoadFloat3(&forward);

    // 正規化
    myVec = DirectX::XMVector3Normalize(myVec);
    fVec = DirectX::XMVector3Normalize(fVec);
    // DirectX::XMMatrixRotationQuaternion()

    // y軸回転 
    //atan2f()
#endif
    

    // 弾が消えるまでの時間を設定する
    eraseTimer = 10.0f;
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
    //GetOwner()->GetTransform()->AddPosition(DirectX::XMFLOAT3(
    //    velocity.x * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor(),
    //    0.0f,
    //    velocity.z * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor()
    //));
    GetOwner()->GetTransform()->AddPosition({
        velocity.x * Argent::Timer::GetDeltaTime(),
        velocity.y * Argent::Timer::GetDeltaTime(),
        velocity.z * Argent::Timer::GetDeltaTime()
        }
    );
}
