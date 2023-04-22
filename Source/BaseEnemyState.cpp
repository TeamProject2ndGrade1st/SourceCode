#include "BaseEnemyState.h"

void EnemyHierarchicalState::SetSubState(int newState)
{
    subState = subStatePool.at(newState);
    subState->Enter();
}

void EnemyHierarchicalState::ChangeSubState(int newState)
{
    subState->Exit();
    SetSubState(newState);
}

void EnemyHierarchicalState::RegisterSubState(EnemyState* state)
{
    subStatePool.emplace_back(state);
}

int EnemyHierarchicalState::GetSubStateIndex()
{
    int i = 0;
    for (EnemyState* state : subStatePool)
    {
        if (subState == state)return i;
        ++i;
    }
    return i;
}
