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
};

