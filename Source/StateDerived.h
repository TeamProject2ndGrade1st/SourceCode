#pragma once
#include "BaseState.h"

class IdleState : public State
{
public:
    IdleState(Argent::Component::BaseActor* chara);
    ~IdleState() {}

    void Enter()override;
    void Execute()override;
    void Exit()override;
};

class MoveOnState

