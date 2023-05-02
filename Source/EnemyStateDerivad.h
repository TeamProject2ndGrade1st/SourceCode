#pragma once
#include "BaseEnemyState.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"

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

    public:
        float attackTime{ 30 };
        bool once = false;  // ノックバック一回だけ入るための変数
                            // falseがまだ入っていない
    };
}

namespace Enemy::Turret
{
    class IdleState :public EnemyState
    {
    public:
        IdleState(BaseEnemy* _friend) :EnemyState(_friend, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

        


    };
}