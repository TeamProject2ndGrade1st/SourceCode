#include "Character.h"

void Character::UpdateVelocity()
{
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        float friction = this->friction * Argent::Timer::GetDeltaTime();
        
        float vx = velocity.x / length;
        float vz = velocity.z / length;
        velocity.x -= vx * friction;
        velocity.z -= vz * friction;
        
    }
    if (length <= maxMoveSpeed)
    {

        float acceleration = this->acceleration * Argent::Timer::GetDeltaTime();

        velocity.x += moveVec.x * acceleration;
        velocity.z += moveVec.z * acceleration;
    }
    //‘¬“x§ŒÀ
    length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > maxMoveSpeed)
    {
        DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
        vec = DirectX::XMVector2Normalize(vec);
        DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxMoveSpeed);
        velocity.x = DirectX::XMVectorGetX(velo);
        velocity.z = DirectX::XMVectorGetY(velo);
    }
    moveVec.x = {};
    moveVec.z = {};
}

void Character::UpdateMove()
{
    GetOwner()->GetTransform()->AddPosition(DirectX::XMFLOAT3(
        velocity.x * Argent::Timer::GetDeltaTime(),
        0.0f,
        velocity.z * Argent::Timer::GetDeltaTime()
    ));
}
