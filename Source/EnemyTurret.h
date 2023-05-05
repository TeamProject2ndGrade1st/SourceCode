#pragma once
#include "BaseEnemy.h"

enum class TurretAnimation
{
    StartUp,
    Attack,
    Idle,
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
};

