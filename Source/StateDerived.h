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

//叫び　基本的に色々な行動の予備動作になる
class ActionState : public State
{
public:
    ActionState(BaseFriend* _friend) : State(_friend, "action") {}
    ~ActionState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

//前進
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

    //アニメーション8~18フレーム間で動きが早くなる
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

