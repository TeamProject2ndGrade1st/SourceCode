#pragma once
#include "BaseEnemy.h"

enum class TurretAnimation
{
    StartUp,    // 起動
    Attack,     // 攻撃
    Idle,       // 待機
    BootWait,   // 起動待機
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
        StartUp,    // 起動
        Attack,     // 攻撃
        Idle,       // 待機
        BootWait,   // 起動待機
    };


};

