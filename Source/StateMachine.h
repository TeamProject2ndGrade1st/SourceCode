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
    //�X�e�[�g�o�^

    int GetStateIndex();

private:
    State* currentState = nullptr;

    //�e�X�e�[�g��ێ�����z��
    std::vector<State*> statePool;
};

