#pragma once
#include "BaseEnemyState.h"


class EnemyIdleState : public EnemyState
{
public:
    // コンストラクタ
    EnemyIdleState(BaseEnemy* _friend) :EnemyState(_friend, "idle") {}
    // デストラクタ
    ~EnemyIdleState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;

    bool SearchFriend(); 

private:
    float searchRange = 10.0f;
};

class EnemyAttackState :public EnemyState
{
public:
    EnemyAttackState(BaseEnemy* _friend) :EnemyState(_friend, "attack") {}
    ~EnemyAttackState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};