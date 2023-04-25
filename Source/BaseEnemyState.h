#pragma once
#include "Argent/Argent.h"
#include <vector>

class BaseEnemy;

class EnemyState
{
public:
    EnemyState(BaseEnemy* _enemy, const char* name) :owner(_enemy), name(name) {}
    virtual ~EnemyState() {}

    // �X�e�[�g�ɓ��������ɌĂ΂��
    virtual void Enter() = 0;

    // �X�e�[�g�Ŗ��t���[���Ă΂��
    virtual void Execute() = 0;

    // �X�e�[�g����o��Ƃ��ɌĂ΂��
    virtual void Exit() = 0;

protected:
    BaseEnemy* owner;
    const char* name;

   
};

class EnemyHierarchicalState :public EnemyState
{
public:
    EnemyHierarchicalState(BaseEnemy* _enemy) :EnemyState(_enemy, "EnemyHierarchical") {}
    virtual ~EnemyHierarchicalState() {}

    virtual void Enter() = 0;
    virtual void Execute() = 0;
    virtual void Exit() = 0;

    virtual void SetSubState(int newState);
    virtual void ChangeSubState(int newState);
    virtual void RegisterSubState(EnemyState* state);
    virtual EnemyState* SetSubState() { return subState; }
    virtual int GetSubStateIndex();

protected:
    std::vector<EnemyState*> subStatePool;
    EnemyState* subState = nullptr;
};

