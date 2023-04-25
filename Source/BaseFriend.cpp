#include "BaseFriend.h"
#include "Argent/Argent.h"
#include "FriendStateDerived.h"

BaseFriend::BaseFriend(const char* name, DirectX::XMFLOAT3 pos) :
    Character(name,pos)
{
    
}


void BaseFriend::Initialize()
{
    Character::Initialize();

    GetOwner()->ReplaceTag(GameObject::Tag::Friend);


   /* GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    
    target = GetOwner()->FindByName("target");
    target->GetTransform()->SetScaleFactor(0.01f);

    BaseActor::Initialize();


    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    GetOwner()->ReplaceTag(GameObject::Tag::Stage);
    //GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);


    stateMachine.reset(new StateMachine);

    stateMachine.get()->RegisterState(new Friend::IdleState(this));
    stateMachine.get()->RegisterState(new Friend::ActionState(this));
    stateMachine.get()->RegisterState(new Friend::WalkState(this));
    stateMachine.get()->RegisterState(new Friend::AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));*/

}

void BaseFriend::Begin()
{

}

void BaseFriend::Update()
{
    if (attackTimer > 0)attackTimer -= Argent::Timer::GetDeltaTime();

    stateMachine.get()->Update();

    UpdateVelocity();
    UpdateMove();
}

void BaseFriend::DrawDebug()
{

    if (ImGui::TreeNode(GetName()))
    {
        if (ImGui::TreeNode("Move"))
        {
            ImGui::SliderFloat("Friction", &friction, 0.0f, 5.0f);
            ImGui::SliderFloat("Acceleration", &acceleration, 0.0f, 10.0f);
            ImGui::InputFloat3("Velocity", &velocity.x);
            ImGui::DragFloat3("TargetPosition", &targetPosition.x,0.1f,-100.0f,100.0f);
            ImGui::TreePop();
        }
        

        /*if (ImGui::TreeNode("State"))
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
        }*/
        
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
    Turn(vx, vz, 180.0f);
}

void BaseFriend::SetAnimation(int index)
{
    
    GameObject* g = GetOwner();
    auto com = g->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();
    
    com->SetAnimation(index);
}

//ƒ^[ƒQƒbƒg‚ªUŒ‚”ÍˆÍ“à‚É‚¢‚é‚©‚Ç‚¤‚©
bool BaseFriend::IsTargetInAttackArea()
{
    float vx = targetPosition.x - GetOwner()->GetTransform()->GetPosition().x;
    float vz = targetPosition.z - GetOwner()->GetTransform()->GetPosition().z;
    float length = sqrtf(vx * vx + vz * vz);
    if (length < attackAreaRadius)return true;
    else return false;
}

