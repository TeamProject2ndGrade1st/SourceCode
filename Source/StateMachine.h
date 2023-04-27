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
    /// �X�e�[�g�o�^
    /// </summary>
    /// �X�e�[�g�}�V�������������N���X�̃C�j�V�����C�U�ŕK�v��
    /// �X�e�[�g�������Ă�
    /// ���ӁFenum�ō�����X�e�[�g�萔�Ɠ������Ԃœo�^���邱��
    /// <returns></returns>

    int GetStateIndex();

private:
    FriendState* currentState = nullptr;

    //�e�X�e�[�g��ێ�����z��
    std::vector<FriendState*> statePool;
};

