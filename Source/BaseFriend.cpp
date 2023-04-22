#include "BaseFriend.h"
#include "Argent/Argent.h"
#include "FriendStateDerived.h"

BaseFriend::BaseFriend():
    Character("BaseFriend")
{
    
}

BaseFriend::BaseFriend(DirectX::XMFLOAT3 setPos)
    : Character("BaseFriend")
{
    GetOwner()->GetTransform()->SetPosition(setPos);
}

void BaseFriend::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    

    BaseActor::Initialize();


    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    GetOwner()->SetTag(GameObject::Tag::Stage);
    //GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);


    stateMachine.reset(new StateMachine);

    stateMachine.get()->RegisterState(new IdleState(this));
    stateMachine.get()->RegisterState(new ActionState(this));
    stateMachine.get()->RegisterState(new WalkState(this));
    stateMachine.get()->RegisterState(new AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
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
}

void BaseFriend::DrawDebug()
{

    if (ImGui::TreeNode(GetName().c_str()))
    {
        if (ImGui::TreeNode("Move"))
        {
            ImGui::SliderFloat("Friction", &friction, 0.0f, 5.0f);
            ImGui::SliderFloat("Acceleration", &acceleration, 0.0f, 10.0f);
            ImGui::InputFloat3("Velocity", &velocity.x);
            ImGui::DragFloat3("TargetPosition", &targetPosition.x,1.0f,-100.0f,100.0f);
            ImGui::TreePop();
        }
        

        if (ImGui::TreeNode("State"))
        {
            switch (stateMachine.get()->GetStateIndex())
            {
            case static_cast<int>(State::Idle):
                ImGui::Text("State Idle");
                break;
            case static_cast<int>(State::Action):
                ImGui::Text("State Action");
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

void BaseFriend::MoveToTarget()
{
    float vx = targetPosition.x - GetOwner()->GetTransform()->GetPosition().x;
    float vz = targetPosition.z - GetOwner()->GetTransform()->GetPosition().z;
    float length = sqrtf(vx * vx + vz * vz);
    vx /= length;
    vz /= length;

    moveVec.x = vx;
    moveVec.z = vz;
}

