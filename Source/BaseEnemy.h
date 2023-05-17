#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
#include "Character.h"
#include "EnemyStateMachine.h"
#include "BaseFriend.h"
#include "FriendManager.h"



class BaseEnemy : public Character
{
public:
    BaseEnemy(const char* name, DirectX::XMFLOAT3 pos);

    virtual ~BaseEnemy() {}

    virtual void Initialize()override;
    virtual void Begin()override;
    virtual void Update()override;
    virtual void DrawDebug()override;

    void SetAnimation(int index)
    {
        GameObject* g = GetOwner();
        //auto com = g->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();


        std::vector<GameObject*> childArray;
        g->GetChildArray(childArray);

        for (auto& c : childArray)
        {
            auto com = c->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();
            com->SetAnimation(index);
        }

        //com->SetAnimation(static_cast<int>(index));
    }

    bool IsAnimationEnd()
    {
        GameObject* g = GetOwner();
        //auto com = g->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();

        std::vector<GameObject*> childArray;
        g->GetChildArray(childArray);

        for (auto& c : childArray)
        {
            auto com = c->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();
            return com->IsAnimationEnd();
        }
        //return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    void SetStateTimer(float timer) { stateTimer = timer; }
    float GetStateTimer() { return stateTimer; }

    void SetTargetPosition(DirectX::XMFLOAT3 pos) { targetPosition = pos; }
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }
    
    EnemyStateMachine* GetStateMachine() { return stateMachine.get(); }

    enum class State
    {
        Idle,
        Attack,
    };

public:
    BaseFriend* _friend;

    BaseFriend* GetFriend() { return _friend; }
    void SetFriend(BaseFriend* f) { _friend = f; }



    BaseFriend* SearchFriend1();
protected:
    // ñ⁄ìIç¿ïW
    DirectX::XMFLOAT3 targetPosition{};

    float stateTimer{};

    std::unique_ptr<EnemyStateMachine> stateMachine = nullptr;
    FriendManager* fManager = nullptr;

    Argent::Component::Renderer::SkinnedMeshRenderer* renderer;
};