#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
#include "Character.h"
#include "StateMachine.h"

enum class FriendAnimation
{
    Idle,
    Action,
    Attack,
    Walk_ChangeFrom_Action,
    Walk,
    Walk_End
};

class BaseFriend : public Character
{
public:
    BaseFriend();
    BaseFriend(DirectX::XMFLOAT3 pos);
    virtual ~BaseFriend() {}

    virtual void Initialize()override;
    virtual void Begin() override;
    virtual void Update()override;
    virtual void DrawDebug()override;

    ////���͏���(�����̂�)
    //void UpdateVelocity();

    ////�ړ�����
    //void UpdateMove();

    //�^�[�Q�b�g�Ɍ������Ĉړ�
    void MoveToTarget();

    void SetAnimation(int index)
    {
        GameObject* g = GetOwner();
        auto com = g->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();

        com->SetAnimation(index);
       
    }
    bool isAnimationEnd()
    {
        return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    void SetStateTimer(float timer) { stateTimer = timer; }
    float GetStateTimer() { return stateTimer; }
    
    void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
    float GetAttackAreaRadius() { return attackAreaRadius; }

    float Init_GetAccelaration() { return init_acceleration; }

    StateMachine* GetStateMachine() { return stateMachine.get(); }

    enum class State
    {
        Idle,
        Action,
        Walk,
        Attack,
    };

protected:
    //�ڕW���W
    DirectX::XMFLOAT3 targetPosition{};

    //�U���n���͈�
    float attackAreaRadius{ 2.0f };
    float attackInterval{ 3.0f };
  
    //�����l
    float init_acceleration{ 0.0f };
    float init_maxMoveSpeed{ 5.0f };
    float init_friction{ 5.0f };

    float stateTimer{};

    std::unique_ptr<StateMachine> stateMachine = nullptr;
};

