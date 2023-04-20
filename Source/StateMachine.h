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

    //ステート登録
    void RegisterState(State* state);

    int GetStateIndex();

private:
    State* currentState = nullptr;

    //各ステートを保持する配列
    std::vector<State*> statePool;
};

