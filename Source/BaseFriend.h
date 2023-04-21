#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
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

    GameObject* GetTarget() { return target; }
    
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

