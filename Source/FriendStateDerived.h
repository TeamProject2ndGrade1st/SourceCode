#pragma once
#include "BaseState.h"

class IdleState : public FriendState
{
public:
    IdleState(BaseFriend* _friend) : FriendState(_friend, "idle") {}
    ~IdleState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//���с@��{�I�ɐF�X�ȍs���̗\������ɂȂ�
class ActionState : public FriendState
{
public:
    ActionState(BaseFriend* _friend) : FriendState(_friend, "action") {}
    ~ActionState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//�O�i
class WalkState : public FriendState
{
public:
    WalkState(BaseFriend* _friend) : FriendState(_friend, "walk") {}
    ~WalkState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;

private:
    float acceleration{ 10.0f };
    float maxSpeed_fast{ 3.0f };
    float maxSpeed_late{ 0.8f };
    float maxSpeed{ 2.0f };

    //�A�j���[�V����20~32�t���[���Ԃœ����������Ȃ�
    int startMovingFastFrame{ 20 };
    int endMovingFastFrame{ 32 };
};

class AttackState : public FriendState
{
public:
    AttackState(BaseFriend* _friend) : FriendState(_friend, "attack") {}
    ~AttackState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//�K�w�\���̃X�e�[�g�i�����j
class H_MoveState : public HierarchicalState
{
public:
    H_MoveState(BaseFriend* _friend) : HierarchicalState(_friend) {}
    ~H_MoveState();

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

class H_BattleState : public HierarchicalState
{
public:
    H_BattleState(BaseFriend* _friend) : HierarchicalState(_friend) {}
    ~H_BattleState();

    void Enter()override;
    void Execute()override;
    void Exit()override;
};
