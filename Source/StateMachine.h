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
    void RegisterState(FriendState* state);

    int GetStateIndex();

private:
    FriendState* currentState = nullptr;

    //各ステートを保持する配列
    std::vector<FriendState*> statePool;
};

