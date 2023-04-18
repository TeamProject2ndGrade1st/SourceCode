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
    //ステート登録

    int GetStateIndex();

private:
    //State* currentState = nullptr;
};

