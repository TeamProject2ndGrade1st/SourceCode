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
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    BaseActor::Initialize();

    GetOwner()->GetTransform()->SetScaleFactor(0.03f);

    stateMachine.reset(new StateMachine);

    stateMachine.get()->RegisterState(new IdleState(this));
    stateMachine.get()->RegisterState(new WalkState(this));
    stateMachine.get()->RegisterState(new AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
}

void BaseFriend::Begin()
{

}

void BaseFriend::Update()
{
    stateMachine.get()->Update();
}

void BaseFriend::DrawDebug()
{
    if (ImGui::TreeNode(GetName().c_str()))
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
        BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}

