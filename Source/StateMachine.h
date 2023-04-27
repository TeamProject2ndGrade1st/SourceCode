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

    void RegisterState(FriendState* state);
    /// <summary>
    /// ステート登録
    /// </summary>
    /// ステートマシンも持たせたクラスのイニシャライザで必要な
    /// ステート分だけ呼ぶ
    /// 注意：enumで作ったステート定数と同じ順番で登録すること
    /// <returns></returns>

    int GetStateIndex();

private:
    FriendState* currentState = nullptr;

    //各ステートを保持する配列
    std::vector<FriendState*> statePool;
};

