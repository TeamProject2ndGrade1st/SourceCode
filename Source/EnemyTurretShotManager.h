#pragma once
#include "Argent/Argent.h"

class EnemyTurretShot;

class EnemyTurretShotManager :
    public Argent::Component::BaseActor
{
public:
    EnemyTurretShotManager() :
        BaseActor("EnemyTurretShotManager") {}
    ~EnemyTurretShotManager() {}

public:
    static EnemyTurretShotManager& Instance()
    {
        static EnemyTurretShotManager instance;
        return instance;
    }

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddShot(const DirectX::XMFLOAT3 pos);

private:
    std::vector<EnemyTurretShot*> shotArray;
};