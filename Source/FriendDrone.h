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
    float huwahuwaDegree{ 0 };//上下に動くための度数
    float huwahuwaSpeed{ 1.5f };//上下に動く速度　
    float init_attackAreaRadius{ 5.0f };//攻撃範囲
};

