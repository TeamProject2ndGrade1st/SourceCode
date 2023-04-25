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
    GetOwner()->SetTag(GameObject::Tag::Friend);

   /* GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    
    target = GetOwner()->FindByName("target");
    target->GetTransform()->SetScaleFactor(0.01f);

    BaseActor::Initialize();


    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    acceleration = init_acceleration;
    maxMoveSpeed = init_maxMoveSpeed;
    friction = init_friction;

    GetOwner()->SetTag(GameObject::Tag::Stage);
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

    if (ImGui::TreeNode(GetName().c_str()))
    {
       
        

        Character::DrawDebug();
        
        
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

void BaseFriend::OnDamaged()
{
}

void BaseFriend::OnDead()
{
}

void BaseFriend::OnHeal()
{
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

bool BaseFriend::SerchEnemy()
{
    std::vector<GameObject*> enemyArray;
    GameObject::FindByTag(GameObject::Tag::Turret, enemyArray);
    if (enemyArray.size() == 0)
    {
        //”­Œ©‚µ‚È‚©‚Á‚½‚ç
        target = nullptr;
        return false;
    }

    float length0 = FLT_MAX;
    for (auto enemy = enemyArray.begin(); enemy != enemyArray.end(); ++enemy)
    {
        DirectX::XMFLOAT3 enemyPos = (*enemy)->GetTransform()->GetPosition();
        DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
        DirectX::XMVECTOR EnemyPos = DirectX::XMLoadFloat3(&enemyPos);

        DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&pos);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(EnemyPos,Pos);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
        float length1;
        DirectX::XMStoreFloat(&length1, Length);
        if (length1 < length0)
        {
            target = (*enemy);
        }
    }
    return true;
}

