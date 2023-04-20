#pragma once
#include "BaseState.h"

class IdleState : public State
{
public:
    IdleState(BaseFriend* _friend) : State(_friend, "idle") {}
    ~IdleState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//���с@��{�I�ɐF�X�ȍs���̗\������ɂȂ�
class ActionState : public State
{
public:
    ActionState(BaseFriend* _friend) : State(_friend, "action") {}
    ~ActionState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//�O�i
class WalkState : public State
{
public:
    WalkState(BaseFriend* _friend) : State(_friend, "walk") {}
    ~WalkState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;

private:
    const float acceleration{ 10.0f };
    const float maxSpeed_fast{ 6.0f };
    const float maxSpeed_late{ 2.0f };

    //�A�j���[�V����8~18�t���[���Ԃœ����������Ȃ�
    const int startMovingFastFrame{ 8 };
    const int endMovingFastFrame{ 18 };
};

class AttackState : public State
{
public:
    AttackState(BaseFriend* _friend) : State(_friend, "attack") {}
    ~AttackState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

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

