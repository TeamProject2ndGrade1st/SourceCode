#pragma once
#include "BaseEnemyState.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"

namespace Enemy::SpikeBot
{

    class IdleState : public EnemyState
    {
    public:
        // �R���X�g���N�^
        IdleState(BaseEnemy* _friend) :EnemyState(_friend, "idle") {}
        // �f�X�g���N�^
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
        bool once = false;  // �m�b�N�o�b�N��񂾂����邽�߂̕ϐ�
                            // false���܂������Ă��Ȃ�
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