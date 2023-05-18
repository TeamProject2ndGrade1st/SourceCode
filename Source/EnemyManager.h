#pragma once
#include "Argent/Scene/BaseScene.h"
#include "BaseFriend.h"

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
        BaseActor("EnemyManager"), scene(currentScene) 
    {
        BaseEnemy::num = 0;
        battleFlag = false;
    }
    ~EnemyManager() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddEnemy(BaseEnemy* _enemy);

    void SetEnemyOnStage();

    BaseEnemy* FindEnemyComponentFromOwner(GameObject* wEnemy)const;

    static bool battleFlag;

private:
    std::vector<BaseEnemy*> enemyArray;
    Argent::Scene::BaseScene* scene;
};

