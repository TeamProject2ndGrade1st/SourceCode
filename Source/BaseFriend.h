#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
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

class BaseFriend
    : public Argent::Component::BaseActor
{
public:

    BaseFriend();
    BaseFriend(DirectX::XMFLOAT3 pos);
    virtual ~BaseFriend() {}

    virtual void Initialize()override;
    virtual void Begin() override;
    virtual void Update()override;
    virtual void DrawDebug()override;

    //ë¨óÕèàóù(êÖïΩÇÃÇ›)
    void UpdateVelocity();

    //à⁄ìÆèàóù
    void UpdateMove();

    void SetAnimation(int index)
    {
        GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->SetAnimation(index);
    }
    bool isAnimationEnd()
    {
        return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    void SetStateTimer(float timer) { stateTimer = timer; }
    float GetStateTimer() { return stateTimer; }
    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }
    void SetAccelaration(float accel) { acceleration = accel; }
    void SetMaxSpeed(float speed) { maxMoveSpeed = speed; }
    void SetFriction(float fric) { friction = fric; }

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
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};
  
    //èâä˙íl
    const float init_acceleration{ 0.0f };
    const float init_maxMoveSpeed{ 5.0f };
    const float init_friction{ 0.5f };
    
    float acceleration{ init_acceleration };
    float maxMoveSpeed{ init_maxMoveSpeed };
    float friction{ init_friction };

    float stateTimer{};

    std::unique_ptr<StateMachine> stateMachine = nullptr;
};

