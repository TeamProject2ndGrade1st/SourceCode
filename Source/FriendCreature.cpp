#include "FriendCreature.h"
#include "FriendStateDerived.h"

FriendCreature::FriendCreature() : BaseFriend("Creature")
{
}

void FriendCreature::Initialize()
{
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));

    //UŒ‚”ÍˆÍ‚ÌŽ‹Šo‰»
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(
        Argent::Component::Collider::RayCastCollider::MeshType::Cylinder
    ));
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = { 
        attackAreaRadius * 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f 
    };

    target = GetOwner()->FindByName("target");
    target->GetTransform()->SetScaleFactor(0.01f);

    BaseActor::Initialize();


    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    GetOwner()->SetTag(GameObject::Tag::Stage);
    GetOwner()->GetTransform()->SetScaleFactor(0.01f);


    stateMachine.reset(new StateMachine);

    stateMachine.get()->RegisterState(new Friend::Creature::IdleState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::ActionState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::WalkState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
}

void FriendCreature::Update()
{
    BaseFriend::Update();
}

void FriendCreature::DrawDebug()
{

    BaseFriend::DrawDebug();

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
}
