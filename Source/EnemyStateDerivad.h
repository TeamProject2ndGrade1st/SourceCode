#pragma once
#include "BaseEnemyState.h"

namespace Enemy::SpikeBot
{

    class IdleState : public EnemyState
    {
    public:
        // コンストラクタ
        IdleState(BaseEnemy* _friend) :EnemyState(_friend, "idle") {}
        // デストラクタ
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

        bool SearchFriend();

    private:
        float searchRange = 50.0f;
    };

    class AttackState :public EnemyState
    {
    public:
        AttackState(BaseEnemy* _friend) :EnemyState(_friend, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };
}