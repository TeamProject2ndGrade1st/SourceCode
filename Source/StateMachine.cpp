#include "StateMachine.h"

StateMachine::~StateMachine()
{
	for (State* state : statePool)
	{
		delete state;
	}
	statePool.clear();
}

void StateMachine::Update()
{
	currentState->Execute();
}

void StateMachine::SetState(int setState)
{
	currentState = statePool.at(setState);
	currentState->Enter();

}

void StateMachine::ChangeState(int newState)
{
	currentState->Exit();
	SetState(newState);
}

void StateMachine::RegisterState(State* state)
{
	statePool.emplace_back(state);
}

int StateMachine::GetStateIndex()
{
	int i = 0;
	for (State* state : statePool)
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
