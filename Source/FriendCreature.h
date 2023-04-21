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
    FriendCreature();
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

