#pragma once
#include "BaseFriend.h"
class FriendDrone : public BaseFriend
{
public:
    FriendDrone(DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f },int route = 0)
        : BaseFriend("Creature", pos,static_cast<Route>(route)) {}
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
    float init_attackAreaRadius{ 400.0f };//UŒ‚”ÍˆÍ
};

