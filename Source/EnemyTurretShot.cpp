#include "EnemyTurretShot.h"

void EnemyTurretShot::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/sphere.fbx"));

    // �X�P�[�����O
    GetOwner()->GetTransform()->SetScaleFactor(0.015f);

    // �ʒu�ݒ�
    init_position.y = 27.0f;
    init_position.z -= 15.0f;
    GetOwner()->GetTransform()->SetPosition(init_position);


    // ���x�ݒ�
    DirectX::XMFLOAT3 moveVec{ 0.0f,0.0f,-8000.0f };
    SetVelocity(moveVec);

    // �e��������܂ł̎��Ԃ�ݒ肷��
    eraseTimer = 2.0f;
}

void EnemyTurretShot::Update()
{
    UpdateMove();   // �ړ�����

    // �e�������J�E���g�_�E��
    eraseTimer -= Argent::Timer::GetDeltaTime();
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
