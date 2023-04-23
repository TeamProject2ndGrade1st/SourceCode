#pragma once
#include <DirectXMath.h>
#include "Character.h"
#include "StateMachine.h"
#include "FriendManager.h"

class BaseFriend : public Character
{
public:
    BaseFriend(const char* name, DirectX::XMFLOAT3 pos);
    virtual ~BaseFriend() {}

    //BaseFriend(DirectX::XMFLOAT3 pos);


    virtual void Initialize()override;
    virtual void Begin() override;
    virtual void Update()override;
    void DrawDebug() override;

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
    float GetStateTimer() const { return stateTimer; }
    void SetAttackTimer(float timer) { attackTimer = timer; }
    float GetAttackTimer() const { return attackTimer; }

    //�����l�Q�b�^�[�A�Z�b�^�[
    float Init_GetAccelaration() const { return init_acceleration; }
    void Init_SetAccelaration(float accel) { init_acceleration = accel; }
    float Init_GetMaxMoveSpeed() const { return init_maxMoveSpeed; }
    void Init_SetMaxMoveSpeed(float speed) { init_maxMoveSpeed = speed; }
    float Init_GetFriction() const { return init_friction; }
    void Init_SetFriction(float friction) { init_friction = friction; }

    
    void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
    DirectX::XMFLOAT3 GetTargetPosition() const { return targetPosition; }
    float GetAttackAreaRadius() const { return attackAreaRadius; }

    StateMachine* GetStateMachine() const { return stateMachine.get(); }
    GameObject* GetTarget() const { return target; }

    void SetTag(FriendManager::Tag tag) { this->tag = tag; }

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

    //�^�O
    FriendManager::Tag tag{};
};

