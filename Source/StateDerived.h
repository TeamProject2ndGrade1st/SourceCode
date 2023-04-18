#pragma once
#include "BaseState.h"

class IdleState : public State
{
public:
    IdleState(BaseFriend* _friend) : State(_friend) {}
    ~IdleState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//���с@��{�I�ɐF�X�ȍs���̗\������ɂȂ�
class ActionState : public State
{
public:
    ActionState(BaseFriend* _friend) : State(_friend) {}
    ~ActionState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//�O�i
class WalkState : public State
{
public:
    WalkState(BaseFriend* _friend) : State(_friend) {}
    ~WalkState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

class AttackState : public State
{
public:
    AttackState(BaseFriend* _friend) : State(_friend) {}
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

