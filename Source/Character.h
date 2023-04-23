#pragma once
#include "Argent/Argent.h"
class Character :
    public Argent::Component::BaseActor
{
public:
    Character(const char* name, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f }) 
        :BaseActor(name) ,init_position(pos) {}
    virtual ~Character() {}

    void Initialize()override;
    
    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }
    void SetAccelaration(float accel) { acceleration = accel; }
    void SetMaxSpeed(float speed) { maxMoveSpeed = speed; }
    void SetFriction(float fric) { friction = fric; }

protected:
    //‘¬—Íˆ—(…•½‚Ì‚İ)
    void UpdateVelocity();

    //ˆÚ“®ˆ—
    void UpdateMove();

    void Turn(float vx, float vz,float rollSpeed);

protected:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};

    float acceleration{ 3.0f };
    float maxMoveSpeed{ 5.0f };
    float friction{ 2.0f };

private:
    //‰ŠúÀ•W
    DirectX::XMFLOAT3 init_position{};
};

