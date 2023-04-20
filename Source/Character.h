#pragma once
#include "Argent/Argent.h"
class Character :
    public Argent::Component::BaseActor
{
public:
    Character(const char* name):BaseActor(name) {}
    virtual ~Character() {}

    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }
    void SetAccelaration(float accel) { acceleration = accel; }
    void SetMaxSpeed(float speed) { maxMoveSpeed = speed; }
    void SetFriction(float fric) { friction = fric; }

protected:
    //���͏���(�����̂�)
    void UpdateVelocity();

    //�ړ�����
    void UpdateMove();



protected:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};

    float acceleration{ 3.0f };
    float maxMoveSpeed{ 5.0f };
    float friction{ 2.0f };
};

