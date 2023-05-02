#pragma once
#include "BaseEnemyState.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"

// スパイクボット
namespace Enemy::SpikeBot
{
    // 待機ステート
    class IdleState : public EnemyState
    {
    public:
        // コンストラクタ
        IdleState(BaseEnemy* _enemy) :EnemyState(_enemy, "idle") {}
        // デストラクタ
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;


    private:
        float searchRange = 50.0f;  // Friendを見つける半径
    };

    // 攻撃ステート
    class AttackState :public EnemyState
    {
    public:
        // コンストラクタ
        AttackState(BaseEnemy* _enemy) :EnemyState(_enemy, "attack") {}
        // デストラクタ
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    public:
        float attackTime{ 30 }; // 攻撃するアニメーションフレーム
        bool once = false;  // ノックバック一回だけ入るための変数
                            // falseがまだ入っていない
    };
}

// タレット
namespace Enemy::Turret
{
    // 待機ステート
    class IdleState :public EnemyState
    {
    public:
        // コンストラクタ
        IdleState(BaseEnemy* _enemy) :EnemyState(_enemy, "idle") {}
        // デストラクタ
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;        
    };

    // 攻撃ステート
    class AttackState :public EnemyState
    {
    public:
        // コンストラクタ
        AttackState(BaseEnemy* _enemy) :EnemyState(_enemy, "attack") {}
        // デストラクタ
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    // 起動ステート
    class StartUpState :public EnemyState
    {
    public:
        // コンストラクタ
        StartUpState(BaseEnemy* _enemy) :EnemyState(_enemy, "startup") {}
        // デストラクタ
        ~StartUpState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };
}