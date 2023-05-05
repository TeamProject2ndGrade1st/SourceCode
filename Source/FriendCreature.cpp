#include "FriendCreature.h"
#include "FriendStateDerived.h"

void FriendCreature::Initialize()
{
    BaseFriend::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver10.fbx", false));
    effectVector.emplace_back(new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/slash.efk", "./Resources/Effects"));
    effectVector.emplace_back(new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/speedup.efk", "./Resources/Effects"));
    for (auto it = effectVector.begin();it != effectVector.end();++it)
    {
        (*it)->scale = { 6.0f,6.0f,6.0f };
        (*it)->rotation.y = -90.0f;
        GetOwner()->AddComponent((*it));
    }


    //攻撃範囲の視覚化
    /*GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(
        Argent::Component::Collider::RayCastCollider::MeshType::Cylinder
    ));
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = { 
        attackAreaRadius * 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f 
    };*/

    //仮置きのターゲット
    //target = GetOwner()->FindByName("target");
    //target->GetTransform()->SetScaleFactor(0.01f);

    //スケール変更
    //GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    

    //タグ付け
    GetOwner()->ReplaceTag(GameObject::Tag::Friend);
    

    //ステートマシンへのステート登録
    stateMachine = std::make_unique<StateMachine>();

    stateMachine.get()->RegisterState(new Friend::Creature::IdleState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::ActionState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::WalkState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::AttackState(this));
    stateMachine.get()->RegisterState(new Friend::Creature::DieState(this));

    stateMachine.get()->SetState(static_cast<int>(State::Idle));
}

void FriendCreature::Begin()
{
    BaseFriend::Begin();
}

void FriendCreature::Update()
{
    //レイキャストコンポーネントでY座標があげられるからその分落とす
    //(なぜかイニシャライザでやっても座標が戻される)
    static bool once;
    auto pos = GetOwner()->GetTransform()->GetPosition();
    if (pos.y >= 0.0f && !once)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.0f, pos.z)); 
        once = true;
    }

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

    if (ImGui::Button("1s HitStop"))
    {
        GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->SetStopTime(1.0f);
    }
    if (relayPoint.size() > 0)
    {
        if (ImGui::TreeNode("RouteSearch"))
        {
            ImGui::SliderFloat3("pointPosition", &relayPoint.at(0).pos.x, -500, 500);
            if (relayPoint.at(0).passage) ImGui::Text("pass");
            else ImGui::Text("no pass");
            ImGui::TreePop();
        }
    }
    
}

void FriendCreature::OnDead()
{
    stateMachine->ChangeState(static_cast<int>(State::Die));
}
