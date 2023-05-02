#include "FriendDrone.h"
#include "FriendStateDerived.h"
#include "StateMachine.h"
#include "Shadow.h"

void FriendDrone::Initialize()
{

    BaseFriend::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0502_1.fbx", false));

    if (myShadow)
    {
        myShadow->GetOwner()->GetTransform()->SetScale(DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f));
    }


    GetOwner()->GetTransform()->SetScaleFactor(0.4f);

    //�U���͈͂̎��o��
    /*GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(
        Argent::Component::Collider::RayCastCollider::MeshType::Cylinder
    ));
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = {
        attackAreaRadius * 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f
    };*/


    //���u���̃^�[�Q�b�g
    //target = GetOwner()->FindByName("target");
    //target->GetTransform()->SetScaleFactor(0.01f);

    acceleration     = init_acceleration;
    maxMoveSpeed     = init_maxMoveSpeed;
    friction         = init_friction;
    attackAreaRadius = init_attackAreaRadius * GetTransform()->GetScaleFactor();

    //�X�e�[�g�}�V���ւ̃X�e�[�g�o�^
    stateMachine = std::make_unique<StateMachine>();

    stateMachine.get()->RegisterState(new Friend::Drone::IdleState(this));
    stateMachine.get()->RegisterState(new Friend::Drone::WalkState(this));
    stateMachine.get()->RegisterState(new Friend::Drone::AttackState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
}

void FriendDrone::Update()
{
    //���C�L���X�g�R���|�[�l���g��Y���W���������邩�炻�̕����Ƃ�
    //(�Ȃ����C�j�V�����C�U�ł���Ă����W���߂����)
    auto pos = GetOwner()->GetTransform()->GetPosition();
    if (pos.y >= 0.0f && !once)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.z));
        once = true;
    }

    BaseFriend::Update();

    //�㉺�ɓ������Ăӂ�ӂ핂�V�����Ă邾��
    pos = GetTransform()->GetPosition();
    huwahuwaDegree += huwahuwaSpeed;
    if (huwahuwaDegree > 360)huwahuwaDegree = 0;
    float huwahuwa = sinf(DirectX::XMConvertToRadians(huwahuwaDegree)) * (20.0f * GetOwner()->GetTransform()->GetScaleFactor());
    GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 80.0f * GetOwner()->GetTransform()->GetScaleFactor() + huwahuwa, pos.z));
}

void FriendDrone::DrawDebug()
{
    BaseFriend::DrawDebug();

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
}
