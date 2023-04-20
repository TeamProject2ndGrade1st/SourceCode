#include "BaseFriend.h"
#include "Argent/Argent.h"
#include "StateDerived.h"

BaseFriend::BaseFriend():
    BaseActor("BaseFriend")
{
    
}

BaseFriend::BaseFriend(DirectX::XMFLOAT3 setPos)
    : BaseActor("BaseFriend")
{
    GetOwner()->GetTransform()->SetPosition(setPos);
}

void BaseFriend::Initialize()
{
    static bool flag;
    if (flag)return;
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    BaseActor::Initialize();

    GetOwner()->GetTransform()->SetScaleFactor(0.03f);

    stateMachine.reset(new StateMachine);

    stateMachine.get()->RegisterState(new IdleState(this));
    stateMachine.get()->RegisterState(new ActionState(this));
    stateMachine.get()->RegisterState(new WalkState(this));
    stateMachine.get()->RegisterState(new AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));

    flag = true;
}

void BaseFriend::Begin()
{

}

void BaseFriend::Update()
{
    DirectX::XMVECTOR moveVec = {
        sinf(DirectX::XMConvertToRadians(GetOwner()->GetTransform()->GetRotation().y)),
        0.0f,
        cosf(DirectX::XMConvertToRadians(GetOwner()->GetTransform()->GetRotation().y))
    };
    moveVec = DirectX::XMVector3Normalize(moveVec);
    DirectX::XMStoreFloat3(&this->moveVec,moveVec);
    stateMachine.get()->Update();

    UpdateVelocity();
    UpdateMove();
    
    //GetOwner()->GetTransform()->AddPosition(velocity);
}

void BaseFriend::DrawDebug()
{
    if (ImGui::TreeNode(GetName().c_str()))
    {
        ImGui::SliderFloat("Friction", &friction,0.0f,1.0f);
        ImGui::SliderFloat("Acceleration", &acceleration,0.0f,10.0f);
        ImGui::InputFloat3("Velocity", &velocity.x);

        if (ImGui::TreeNode("State"))
        {
            switch (stateMachine.get()->GetStateIndex())
            {
            case static_cast<int>(State::Idle):
                ImGui::Text("State Idle");
                break;
            case static_cast<int>(State::Walk):
                ImGui::Text("State Walk");
                break;
            case static_cast<int>(State::Attack):
                ImGui::Text("State Attack");
                break;
            }
            ImGui::SliderFloat("State Timer", &stateTimer, 0.0f, 30.0f);
            ImGui::TreePop();
        }
        BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}

void BaseFriend::UpdateVelocity()
{
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
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

void BaseFriend::UpdateMove()
{
    GetOwner()->GetTransform()->AddPosition(DirectX::XMFLOAT3(
        velocity.x * Argent::Timer::GetDeltaTime(),
        0.0f,
        velocity.z * Argent::Timer::GetDeltaTime()
    ));
}

