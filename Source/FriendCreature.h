#pragma once
#include "BaseFriend.h"
#include <vector>

enum class CreatureAnimation
{
    Idle,
    Action,
    Attack,
    Walk_ChangeFrom_Action,
    Walk,
    Walk_End,
    Die
};

class FriendCreature :
    public BaseFriend
{
public:
    static int num;

    FriendCreature(DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f},int route = 0) 
        : BaseFriend("Creature", pos, static_cast<Route>(route)) {
        num++;
    }
    ~FriendCreature() { num--; }
    
    void Initialize() override;
    void Update() override;
    void DrawDebug() override;

    void OnDead()override;


    enum class State
    {
        Idle,
        Action,
        Walk,
        Attack,
        Die,
    };

private:
    std::vector<Argent::Component::Renderer::EffekseerEmitter*> effectVector;

    FriendManager* friendManager{ nullptr };
};

