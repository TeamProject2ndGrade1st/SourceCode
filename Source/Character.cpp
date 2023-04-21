#include "Character.h"

void Character::UpdateVelocity()
{
    if (moveVec.x == 0.0f && moveVec.z == 0.0f)
    {
        DirectX::XMVECTOR moveVec = {
        sinf(DirectX::XMConvertToRadians(GetOwner()->GetTransform()->GetRotation().y)),
        0.0f,
        cosf(DirectX::XMConvertToRadians(GetOwner()->GetTransform()->GetRotation().y))
        };

        moveVec = DirectX::XMVector3Normalize(moveVec);
        DirectX::XMStoreFloat3(&this->moveVec, moveVec);
    }

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

void Character::Turn(float vx, float vz,float rollSpeed)
{
    rollSpeed *= Argent::Timer::GetDeltaTime();

    if (vx == 0.0f && vz == 0.0f)return;
    
    DirectX::XMVECTOR vec{ vx,vz };
    DirectX::XMVECTOR Length = DirectX::XMVector2Length(vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);

    DirectX::XMVECTOR front{ GetOwner()->GetTransform()->CalcForward().x,GetOwner()->GetTransform()->CalcForward().z };
    DirectX::XMVECTOR Dot = DirectX::XMVector2Dot(vec, front);
    float dot;
    DirectX::XMStoreFloat(&dot, Dot);
    float rot = 1.0f - dot;
    rollSpeed *= rot;

    //ŠOÏ‚Å¶‰E”»’è
    DirectX::XMVECTOR Cross = DirectX::XMVector2Cross(vec, front);
    float cross;
    DirectX::XMStoreFloat(&cross, Cross);
    DirectX::XMFLOAT4 rotation = GetOwner()->GetTransform()->GetRotation();
    rotation.y += (cross < 0.0f) ? -rollSpeed : rollSpeed;
    GetOwner()->GetTransform()->SetRotation(rotation);
    
}
