#pragma once
class StateMachine
{
public:
    StateMachine() {}
    ~StateMachine();

    void Update();
    void SetState();
    void ChangeState();

    void RegisterState();
    //�X�e�[�g�o�^

    int GetStateIndex();

private:
    //State* currentState = nullptr;
};

