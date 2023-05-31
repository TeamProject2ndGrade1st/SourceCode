#include "BaseFriend.h"
#include "FriendStateDerived.h"
#include "EnemyManager.h"

int BaseFriend::num = 0;

BaseFriend::BaseFriend(const char* name, DirectX::XMFLOAT3 pos, Route route) : 
    Character(name, pos)
{
    num++;
    //RouteSearch(relayPoint, route);
}

void BaseFriend::Initialize()
{
    Character::Initialize();

    GetOwner()->ReplaceTag(GameObject::Tag::Friend);

    GetOwner()->GetTransform()->SetScaleFactor(0.15f);
   
    if (!eManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::EnemyManager, g);
        if (g.size() > 0)
            eManager = g.at(0)->GetComponent<EnemyManager>();
    }

    attackAreaRadius = init_attackAreaRadius * GetTransform()->GetScaleFactor();

    friendManager = GameObject::FindByName("FriendManager")->GetComponent<FriendManager>();

    GetOwner()->AddComponent(healEffect = new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/heal.efk","./Resources/Effects"));
    healEffect->offset.y = 17.5f;
    healEffect->scale = { 10,10,10 };
    GetOwner()->AddComponent(spdUpEffect = new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/speedup.efk","./Resources/Effects"));
    spdUpEffect->scale = { 6,6,6 };
}

void BaseFriend::Begin()
{
    Character::Begin();

    if (!eManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::EnemyManager, g);
        if (g.size() > 0)
            eManager = g.at(0)->GetComponent<EnemyManager>();
    }
}

void BaseFriend::Update()
{
    if (!FriendManager::battleFlag)return;

    if (attackTimer > 0)attackTimer -= Argent::Timer::GetDeltaTime();

    stateMachine.get()->Update();

    UpdateVelocity();
    UpdateMove();
}

void BaseFriend::DrawDebug()
{

    if (ImGui::TreeNode(GetName()))
    {
        Character::DrawDebug();
        
        ImGui::TreePop();
    }
}

void BaseFriend::MoveToTarget()
{
    if (!target)return;
    
    float vx = target->GetOwner()->GetTransform()->GetPosition().x - GetOwner()->GetTransform()->GetPosition().x;
    float vz = target->GetOwner()->GetTransform()->GetPosition().z - GetOwner()->GetTransform()->GetPosition().z;
    float length = sqrtf(vx * vx + vz * vz);

    vx /= length;
    vz /= length;
    
    

    ////“G‚æ‚è‚à’Ê‰ßƒ|ƒCƒ“ƒg‚ª‹ß‚¢ê‡‚Í‚»‚Á‚¿‚Éi‚Þ
    //if (relayPoint.size() > 0)
    //{
    //    for (auto it = relayPoint.begin(); it != relayPoint.end(); ++it)
    //    {
    //        if ((*it).passage)continue;
    //        float vxR = (*it).pos.x - GetOwner()->GetTransform()->GetPosition().x;
    //        float vzR = (*it).pos.z - GetOwner()->GetTransform()->GetPosition().z;
    //        float lengthR = sqrtf(vxR * vxR + vzR * vzR);
    //        if (lengthR < 10.0f)
    //        {
    //            (*it).passage = true;
    //            continue;
    //        }

    //        if (lengthR < length)
    //        {
    //            vx = vxR / lengthR;
    //            vz = vzR / lengthR;
    //        }
    //        break;
    //    }
    //}

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
    GetOwner()->SetActive(false);
    num--;
}

void BaseFriend::OnHeal()
{
    healEffect->OnPlay();
}

//ƒ^[ƒQƒbƒg‚ªUŒ‚”ÍˆÍ“à‚É‚¢‚é‚©‚Ç‚¤‚©
bool BaseFriend::IsTargetInAttackArea()
{
    if (!target)return false;
    auto collider = Argent::Component::Collider::SphereCollider(GetAttackAreaRadius());
    
    return target->GetOwner()->GetComponent<Argent::Component::Collider::SphereCollider>()->DetectionOnly(&collider,GetOwner()->GetTransform());
    /*float vx = target->GetOwner()->GetTransform()->GetPosition().x - GetOwner()->GetTransform()->GetPosition().x;
    float vz = target->GetOwner()->GetTransform()->GetPosition().z - GetOwner()->GetTransform()->GetPosition().z;
    float length = sqrtf(vx * vx + vz * vz);
    if (length < attackAreaRadius)return true;
    else return false;*/
}

bool BaseFriend::SearchEnemy()
{
    std::vector<GameObject*> enemyArray;
    std::vector<GameObject*> enemyManager;
    GameObject::FindByTag(GameObject::Tag::Enemy, enemyArray);
    GameObject::FindByTag(GameObject::Tag::EnemyManager, enemyManager);
    if (enemyArray.size() == 0)
    {
        //”­Œ©‚µ‚È‚©‚Á‚½‚ç
        target = nullptr;
        return false;
    }

    target = eManager->FindEnemyComponentFromOwner((enemyArray.at(0)));

    //’Ê‰ßƒ|ƒCƒ“ƒg‚Æ“G‚Ü‚Å‚Ì‹——£‚ð”äŠr
    //“G‚Ì•û‚ª‹ß‚¢‚È‚çƒŒƒCƒLƒƒƒXƒg‚ÅáŠQ•¨ŒŸõ
    float length0 = FLT_MAX;
    int i = 1;
    for (auto enemy = enemyArray.begin()+1; enemy != enemyArray.end(); ++enemy,++i)
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
            target = eManager->FindEnemyComponentFromOwner(enemyArray.at(i));
            length0 = length1;
        }
    }
    return true;
}

//bool BaseFriend::SearchTarget()
//{
//    if (target)return true;
//
//    for (auto it = relayPoint.begin(); it != relayPoint.end(); ++it)
//    {
//        //’Ê‰ß‚µ‚Ä‚¢‚È‚¯‚ê‚Î
//        if (!(*it).passage)return true;
//    }
//    
//    return false;
//}

//void BaseFriend::RouteSearch(std::vector<RelayPoint>& point,Route route)
//{
//    switch (route)
//    {
//    case BaseFriend::left:
//        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
//        point.emplace_back(RelayPoint{ { -65, 0, -100 }, false });
//        point.emplace_back(RelayPoint{ { -110, 0, -50 }, false });
//        point.emplace_back(RelayPoint{ { -110, 0, 150 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
//        break;
//    case BaseFriend::center:
//        point.emplace_back(RelayPoint{ { 0, 0, -300 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, 0 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, 150 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
//        break;
//    case BaseFriend::right:
//        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
//        point.emplace_back(RelayPoint{ { 65, 0, -100 }, false });
//        point.emplace_back(RelayPoint{ { 110, 0, -50 }, false });
//        point.emplace_back(RelayPoint{ { 110, 0, 150 }, false });
//        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
//        break;
//    }
//}

