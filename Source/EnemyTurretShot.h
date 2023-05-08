#pragma once
#include "Argent/Argent.h"

class EnemyTurretShot :
    public Argent::Component::BaseActor
{
public:
    // �R���X�g���N�^
    EnemyTurretShot(const char* name, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f })
        :BaseActor(name), init_position(pos) {}
    // �f�X�g���N�^
    ~EnemyTurretShot() {}

    void Initialize()override;  // �ŏ��̈�񂾂��Ă΂��
    void Update()override;      // ���t���[��begin��end�̊ԂɌĂ΂��
    void DrawDebug()override;

    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }



    // �ړ�����
    void UpdateMove();

public:
    float eraseTimer = {};  // �e��������܂ł̎���

private:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};

private:
    // �������W�ʒu
    DirectX::XMFLOAT3 init_position{};
};

