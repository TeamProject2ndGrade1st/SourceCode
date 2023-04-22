#include "EnemyStateMachine.h"

// �R���X�g���N�^
EnemyStateMachine::~EnemyStateMachine()
{
    for (EnemyState* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}

// �X�V
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

    //�X�e�[�g��������Ȃ������Ƃ�
    return -1;
}
