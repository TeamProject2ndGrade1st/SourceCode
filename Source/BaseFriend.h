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
    ~BaseFriend() {}

    void Initialize()override;
    void Begin() override;
    void Update()override;
    void DrawDebug()override;

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

    StateMachine* GetStateMachine() { return stateMachine.get(); }

    enum class State
    {
        Idle,
        Action,
        Walk,
        Attack,
    };

private:
    DirectX::XMFLOAT3 speed = {0,0,0.02f};
    float stateTimer{};

    std::unique_ptr<StateMachine> stateMachine = nullptr;
};

