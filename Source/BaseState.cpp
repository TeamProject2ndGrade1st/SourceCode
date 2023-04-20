#include "BaseState.h"

void HierarchicalState::SetSubState(int newState)
{
    subState = subStatePool.at(newState);
    subState->Enter();
}

void HierarchicalState::ChangeSubState(int newState)
{
    subState->Exit();
    SetSubState(newState);
}

void HierarchicalState::RegisterSubState(State* state)
{
    subStatePool.emplace_back(state);
}

int HierarchicalState::GetSubStateIndex()
{
    int i = 0;
    for (State* state : subStatePool)
    {
        if (subState == state)return i;
        ++i;
    }
    return i;
}
