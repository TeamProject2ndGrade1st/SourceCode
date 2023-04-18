#pragma once
#include "BaseState.h"

class StateMachine
{
public:
    StateMachine() {}
    ~StateMachine();

    void Update();
    void SetState(int setState);
    void ChangeState(int newState);

    void RegisterState(State* state);
    //ステート登録

    int GetStateIndex();

private:
    State* currentState = nullptr;

    //各ステートを保持する配列
    std::vector<State*> statePool;
};

