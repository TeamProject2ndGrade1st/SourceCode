#pragma once
#include "BaseEnemy.h"
#include "Argent/Argent.h"
class EnemySpikeBot :
    public BaseEnemy
{
public:
    EnemySpikeBot();
    ~EnemySpikeBot() {}

    void Initialize()override;
    void Begin()override;
    void Update()override;
    void DrawDebug()override;

    
};

