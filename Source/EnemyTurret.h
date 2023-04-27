#pragma once
#include "BaseEnemy.h"
class EnemyTurret :
    public BaseEnemy
{
public:
    EnemyTurret();
    ~EnemyTurret() {}

    void Initialize()override;
    void Begin()override;
    void Update()override;
    void DrawDebug()override;
};

