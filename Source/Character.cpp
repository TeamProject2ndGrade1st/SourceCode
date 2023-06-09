#include "Character.h"
#include "Argent/Argent.h"
#include "Shadow.h"


void Character::Initialize()
{
    BaseActor::Initialize();
    
    //影の表示

    //myShadow = new GameObject("Shadow", new Shadow(this));
    //GetOwner()->AddChild(myShadow);

    

    GetOwner()->AddComponent(new Argent::Component::Collider::SphereCollider(10.0f));

    GetOwner()->GetTransform()->SetPosition(init_position);
}

void Character::DrawDebug()
{
    if (ImGui::TreeNode("Status"))
    {
        ImGui::SliderFloat("HP", &health, 0, maxHealth,"%.2f");
        ImGui::SliderFloat("MAX_HP", &maxHealth, 0, 50,"%.2f");
        ImGui::SliderFloat("Attack", &attack, 0, 30,"%.2f");
        ImGui::SliderFloat("Weight", &weight, 0, 100,"%.2f");
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Move"))
    {
        ImGui::SliderFloat("Friction", &friction, 0.0f, 1000.0f);
        ImGui::SliderFloat("Acceleration", &acceleration, 0.0f, 1000.0f);
        ImGui::SliderFloat("MaxSpeed", &maxMoveSpeed, 0.0f, 1000.0f);
        ImGui::InputFloat3("Velocity", &velocity.x);
        ImGui::TreePop();
    }

    BaseActor::DrawDebug();
}

void Character::Begin()
{
    if (!initOnce)
    {
        GetTransform()->SetPosition(init_position);
        initOnce = true;
    }
}

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
    //速度制限
    length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    //強すぎる力が働いたときは速度制限をしない
    if (length > maxMoveSpeed && length <= maxMoveSpeed + 10.0f)
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
        velocity.x * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor(),
        0.0f,
        velocity.z * Argent::Timer::GetDeltaTime() * GetTransform()->GetScaleFactor()
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

    //外積で左右判定
    DirectX::XMVECTOR Cross = DirectX::XMVector2Cross(vec, front);
    float cross;
    DirectX::XMStoreFloat(&cross, Cross);
    DirectX::XMFLOAT4 rotation = GetOwner()->GetTransform()->GetRotation();
    rotation.y += (cross < 0.0f) ? -rollSpeed : rollSpeed;
    GetOwner()->GetTransform()->SetRotation(rotation);
    
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //体重による抵抗値
    float registance = 1.0f - weight * 0.01f;
    DirectX::XMVECTOR Impulse = {
        impulse.x * registance,
        impulse.y * registance,
        impulse.z * registance
    };

    //速力に力を加える
    DirectX::XMVECTOR Velocity = DirectX::XMLoadFloat3(&velocity);
    Velocity = DirectX::XMVectorAdd(Velocity, Impulse);
    DirectX::XMStoreFloat3(&velocity, Velocity);
}

bool Character::ApplyDamage(float damage)
{
    if (damage <= 0)return false;
    if (health <= 0)return false;
    
    health -= damage;
    if (health <= 0)
    {
       OnDead();
    }
    else
    {
        OnDamaged();
    }

    return true;
}

bool Character::ApplyHeal(float heal)
{
    if (heal <= 0)return false;
    if (health >= maxHealth)return false;

    health += heal;

    OnHeal();
    
    return true;
}