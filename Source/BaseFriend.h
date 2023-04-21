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

    ////速力処理(水平のみ)
    //void UpdateVelocity();

    ////移動処理
    //void UpdateMove();

    //ターゲットに向かって移動
    void MoveToTarget();

    void SetAnimation(int index);
    bool IsAnimationEnd()
    {
        return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    //ターゲットが攻撃範囲内にいるか
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
    //目標座標
    DirectX::XMFLOAT3 targetPosition{};
    GameObject* target{ nullptr };

    //攻撃始動範囲
    float attackAreaRadius{ 2.0f };
  
    //初期値
    float init_acceleration{ 0.0f };
    float init_maxMoveSpeed{ 5.0f };
    float init_friction{ 5.0f };

    float stateTimer{};

    //攻撃後のインターバル計測用
    float attackTimer{};

    std::unique_ptr<StateMachine> stateMachine{ nullptr };
};

