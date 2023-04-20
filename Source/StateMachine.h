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

    //�X�e�[�g�o�^
    void RegisterState(State* state);

    int GetStateIndex();

private:
    State* currentState = nullptr;

    //�e�X�e�[�g��ێ�����z��
    std::vector<State*> statePool;
};

