#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
#include "Argent/Argent.h"
#include "Character.h"
#include "StateMachine.h"

class BaseFriend : public Character
{
public:
    BaseFriend(const char* name);
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

    void SetAnimation(int index);
    bool IsAnimationEnd()
    {
        return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    //�^�[�Q�b�g���U���͈͓��ɂ��邩
    bool IsTargetInAttackArea();

    void SetStateTimer(float timer) { stateTimer = timer; }
    float GetStateTimer() { return stateTimer; }
    void SetAttackTimer(float timer) { attackTimer = timer; }
    float GetAttackTimer() { return attackTimer; }

    float Init_GetAccelaration() { return init_acceleration; }
    void Init_SetAccelaration(float accel) { init_acceleration = accel; }
    float Init_GetMaxMoveSpeed() { return init_maxMoveSpeed; }
    void Init_SetMaxMoveSpeed(float speed) { init_maxMoveSpeed = speed; }
    float Init_GetFriction() { return init_friction; }
    void Init_SetFriction(float fric) { init_friction = fric; }

    
    void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
    float GetAttackAreaRadius() { return attackAreaRadius; }

    StateMachine* GetStateMachine() { return stateMachine.get(); }
    GameObject* GetTarget() { return target; }

    

protected:
    //�ڕW���W
    DirectX::XMFLOAT3 targetPosition{};
    GameObject* target{ nullptr };

    //�U���n���͈�
    float attackAreaRadius{ 2.0f };
  
    //�����l
    float init_acceleration{ 0.0f };
    float init_maxMoveSpeed{ 5.0f };
    float init_friction{ 5.0f };

    float stateTimer{};

    //�U����̃C���^�[�o���v���p
    float attackTimer{};

    std::unique_ptr<StateMachine> stateMachine{ nullptr };
};

