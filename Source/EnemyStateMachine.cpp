#include "EnemyStateMachine.h"

// コンストラクタ
EnemyStateMachine::~EnemyStateMachine()
{
    for (EnemyState* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

// 更新
void EnemyStateMachine::Update()
{
    currentState->Execute();
}

void EnemyStateMachine::SetState(int setState)
{
    currentState = statePool.at(setState);
    currentState->Enter();
}

void EnemyStateMachine::ChangeState(int newState)
{
    currentState->Exit();
    SetState(newState);
}

void EnemyStateMachine::RegisterState(EnemyState* state)
{
    statePool.emplace_back(state);
}

int EnemyStateMachine::GetStateIndex()
{
    int i = 0;
    for (EnemyState* state : statePool)
    {
        if (state == currentState)
        {
            return i;
        }
        ++i;
    }

    //ステートが見つからなかったとき
    return -1;
}
