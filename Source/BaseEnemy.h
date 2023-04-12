#pragma once
#include <DirectXMath.h>
#include "Argent/Component/BaseActor.h"

class BaseEnemy
    : public Argent::Component::BaseActor
{
public:

    BaseEnemy();
    BaseEnemy(DirectX::XMFLOAT3 pos);
    ~BaseEnemy() {}

    void Initialize()override;
    void Begin() override;

    void Update()override;

private:
    DirectX::XMFLOAT3 speed = {0,0,0.02f};
};

