#pragma once
#include "BaseEnemy.h"

enum class TurretAnimation
{
    StartUp,    // �N��
    Attack,     // �U��
    Idle,       // �ҋ@
    BootWait,   // �N���ҋ@
};

class EnemyTurret :
    public BaseEnemy
{
public:
    EnemyTurret(DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f })
        : BaseEnemy("EnemyTurret", pos) {}
    ~EnemyTurret() {}

    void Initialize()override;
    void Begin()override;
    void Update()override;
    void DrawDebug()override;

    enum class State
    {
        StartUp,    // �N��
        Attack,     // �U��
        Idle,       // �ҋ@
        BootWait,   // �N���ҋ@
    };


};

