#include "FriendCreature.h"
#include "FriendStateDerived.h"


void FriendCreature::Initialize()
{
    //�X�P�[���ύX
    GetOwner()->GetTransform()->SetScaleFactor(0.01f);

    BaseFriend::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));

    //�e�̕\��
    //GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/shadow0425_3.fbx", false));
    
    
    //�U���͈͂̎��o��
    /*GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(
        Argent::Component::Collider::RayCastCollider::MeshType::Cylinder
    ));
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = { 
        attackAreaRadius * 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f 
    };*/

    
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    //�^�O�t��
    GetOwner()->SetTag(GameObject::Tag::Friend);

    //�X�e�[�g�}�V���ւ̃X�e�[�g�o�^
    stateMachine = std::make_unique<StateMachine>();

    stateMachine.get()->RegisterState(new Friend::Creature::IdleState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::ActionState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::WalkState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
}

void FriendCreature::Update()
{
    //���C�L���X�g�R���|�[�l���g��Y���W���������邩�炻�̕����Ƃ�
    //(�Ȃ����C�j�V�����C�U�ł���Ă����W���߂����)
    /*static bool once;
    auto pos = GetOwner()->GetTransform()->GetPosition();
    if (pos.y >= 0.0f && !once)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.z)); 
        once = true;
    }*/

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
