#pragma once
#include "BaseEnemyState.h"

class EnemyStateMachine
{
public:
    EnemyStateMachine() {}
    ~EnemyStateMachine();

    void Update();
    void SetState(int setState);
    void ChangeState(int newState);

    // ステート登録
    void RegisterState(EnemyState* state);

    int GetStateIndex();

private:
    EnemyState* currentState = nullptr;

    // 各ステートを保持する配列
    std::vector<EnemyState*> statePool;
};

