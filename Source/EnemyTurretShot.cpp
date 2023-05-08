#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike.fbx"));

    // �X�P�[�����O
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);

    // ���x�ݒ�
    DirectX::XMFLOAT3 moveVec{ 0.0f,0.0f,-100.0f };
    SetVelocity(moveVec);
}

void EnemyTurretShot::Update()
{
    UpdateMove();   // �ړ�����
}

void EnemyTurretShot::DrawDebug()
{
}

// �ړ�����
void EnemyTurretShot::UpdateMove()
{
    GetOwner()->GetTransform()->AddPosition(DirectX::XMFLOAT3(
        velocity.x * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor(),
        0.0f,
        velocity.z * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor()
    ));
}
