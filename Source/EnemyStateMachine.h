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

    // �X�e�[�g�o�^
    void RegisterState(EnemyState* state);

    int GetStateIndex();

private:
    EnemyState* currentState = nullptr;

    // �e�X�e�[�g��ێ�����z��
    std::vector<EnemyState*> statePool;
};

