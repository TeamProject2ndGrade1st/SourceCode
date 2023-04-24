#pragma once
#include "BaseFriend.h"
class FriendDrone : public BaseFriend
{
public:
    FriendDrone(DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f })
        : BaseFriend("Creature", pos) {}
    ~FriendDrone() {}

    void Initialize() override;
    void Update() override;
    void DrawDebug() override;

    enum class State
    {
        Idle,
        Walk,
        Attack,
    };

private:
    float huwahuwaDegree{ 0 };//ã‰º‚É“®‚­‚½‚ß‚Ì“x”
    float huwahuwaSpeed{ 1.5f };//ã‰º‚É“®‚­‘¬“x@
    float init_attackAreaRadius{ 5.0f };//UŒ‚”ÍˆÍ
};

