#pragma once
#include "BaseEnemyState.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"

// �X�p�C�N�{�b�g
namespace Enemy::SpikeBot
{
    // �ҋ@�X�e�[�g
    class IdleState : public EnemyState
    {
    public:
        // �R���X�g���N�^
        IdleState(BaseEnemy* _enemy) :EnemyState(_enemy, "idle") {}
        // �f�X�g���N�^
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;


    private:
        float searchRange = 50.0f;  // Friend�������锼�a
    };

    // �U���X�e�[�g
    class AttackState :public EnemyState
    {
    public:
        // �R���X�g���N�^
        AttackState(BaseEnemy* _enemy) :EnemyState(_enemy, "attack") {}
        // �f�X�g���N�^
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    public:
        float attackTime{ 30 }; // �U������A�j���[�V�����t���[��
        bool once = false;  // �m�b�N�o�b�N��񂾂����邽�߂̕ϐ�
                            // false���܂������Ă��Ȃ�
    };
}

// �^���b�g
namespace Enemy::Turret
{
    // �ҋ@�X�e�[�g
    class IdleState :public EnemyState
    {
    public:
        // �R���X�g���N�^
        IdleState(BaseEnemy* _enemy) :EnemyState(_enemy, "idle") {}
        // �f�X�g���N�^
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;        
    };

    // �U���X�e�[�g
    class AttackState :public EnemyState
    {
    public:
        // �R���X�g���N�^
        AttackState(BaseEnemy* _enemy) :EnemyState(_enemy, "attack") {}
        // �f�X�g���N�^
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    // �N���X�e�[�g
    class StartUpState :public EnemyState
    {
    public:
        // �R���X�g���N�^
        StartUpState(BaseEnemy* _enemy) :EnemyState(_enemy, "startup") {}
        // �f�X�g���N�^
        ~StartUpState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };
}