#pragma once
#include "Argent/Scene/BaseScene.h"

class BaseEnemy;

class EnemyManager : public Argent::Component::BaseActor
{
public:
    enum class Type
    {
        Spikebot,
        Turret,
        End,
    };

    EnemyManager(Argent::Scene::BaseScene* currentScene) :
        BaseActor("EnemyManager"), scene(currentScene) {}
    ~EnemyManager() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddEnemy(BaseEnemy* _enemy);

    BaseEnemy* FindEnemyComponentFromOwner(GameObject* wEnemy)const;

private:
    std::vector<BaseEnemy*> enemyArray;
    Argent::Scene::BaseScene* scene;
};

