#pragma once
#include "BaseFriend.h"

enum class CreatureAnimation
{
    Idle,
    Action,
    Attack,
    Walk_ChangeFrom_Action,
    Walk,
    Walk_End
};

class FriendCreature :
    public BaseFriend
{
public:
    FriendCreature(DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f});
    ~FriendCreature() {}
    
    void Initialize() override;
    void Update() override;
    void DrawDebug() override;

    enum class State
    {
        Idle,
        Action,
        Walk,
        Attack,
    };

    float Init_GetAccelaration() { return init_acceleration; }

private:
};

