#include "BaseFriend.h"
#include "FriendStateDerived.h"

BaseFriend::BaseFriend(const char* name, DirectX::XMFLOAT3 pos, Route route) : 
    Character(name, pos)
{
    RouteSearch(relayPoint, route);
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
    if (!SearchTarget())return;
    float vx = moveVec.x;
    float vz = moveVec.z;
    float length = FLT_MAX;

    if (target)
    {
        vx = target->GetOwner()->GetTransform()->GetPosition().x - GetOwner()->GetTransform()->GetPosition().x;
        vz = target->GetOwner()->GetTransform()->GetPosition().z - GetOwner()->GetTransform()->GetPosition().z;
        length = sqrtf(vx * vx + vz * vz);

        vx /= length;
        vz /= length;
    }
    

    //敵よりも通過ポイントが近い場合はそっちに進む
    if (relayPoint.size() > 0)
    {
        for (auto it = relayPoint.begin(); it != relayPoint.end(); ++it)
        {
            if ((*it).passage)continue;
            float vxR = (*it).pos.x - GetOwner()->GetTransform()->GetPosition().x;
            float vzR = (*it).pos.z - GetOwner()->GetTransform()->GetPosition().z;
            float lengthR = sqrtf(vxR * vxR + vzR * vzR);
            if (lengthR < 10.0f)
            {
                (*it).passage = true;
                continue;
            }

            if (lengthR < length)
            {
                vx = vxR / lengthR;
                vz = vzR / lengthR;
            }
            break;
        }
    }

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

//ターゲットが攻撃範囲内にいるかどうか
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
        //発見しなかったら
        target = nullptr;
        return false;
    }

    target = eManager->FindEnemyComponentFromOwner((enemyArray.at(0)));

    //通過ポイントまでの距離を保持しておく
    float length0 = FLT_MAX;
    if (relayPoint.size() > 0)
    {
        for (auto it = relayPoint.begin(); it != relayPoint.end(); ++it)
        {
            if ((*it).passage)continue;
            float vxR = (*it).pos.x - GetOwner()->GetTransform()->GetPosition().x;
            float vzR = (*it).pos.z - GetOwner()->GetTransform()->GetPosition().z;
            length0 = sqrtf(vxR * vxR + vzR * vzR);
            break;
        }
    }

    //通過ポイントと敵までの距離を比較
    //敵の方が近いならレイキャストで障害物検索
    target = nullptr;
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
            //自分から目標までの間に障害物がないか
            /*std::vector<GameObject*> stage;
            GetOwner()->FindByTag(GameObject::Tag::Stage, stage);*/
            auto stage = GetOwner()->FindByName("Main Stage");
            DirectX::XMFLOAT4X4 world = GetTransform()->GetWorld();
            DirectX::XMMATRIX World = DirectX::XMLoadFloat4x4(&world);
            Argent::Component::Collision::HitResult result;

            //地面すれすれ怖いから上げとく
            pos.y = 10;
            enemyPos.y = 10;

            if (!Argent::Helper::Collision::IntersectRayVsModel(
                pos, enemyPos,
                stage->GetComponent<Argent::Component::Renderer::MeshRenderer>()->GetMesh()->meshResource,
                World,result
            ))
            {
                //障害物がなかった
                target = eManager->FindEnemyComponentFromOwner((*enemy));
            }
        }
    }
    return true;
}

bool BaseFriend::SearchTarget()
{
    if (target)return true;

    for (auto it = relayPoint.begin(); it != relayPoint.end(); ++it)
    {
        //通過していなければ
        if (!(*it).passage)return true;
    }
    
    return false;
}

void BaseFriend::RouteSearch(std::vector<RelayPoint>& point,Route route)
{
    switch (route)
    {
    case BaseFriend::left:
        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
        point.emplace_back(RelayPoint{ { -65, 0, -100 }, false });
        point.emplace_back(RelayPoint{ { -110, 0, -50 }, false });
        point.emplace_back(RelayPoint{ { -110, 0, 150 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
        break;
    case BaseFriend::center:
        point.emplace_back(RelayPoint{ { 0, 0, -300 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, 0 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, 150 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
        break;
    case BaseFriend::right:
        point.emplace_back(RelayPoint{ { 0, 0, -150 }, false });
        point.emplace_back(RelayPoint{ { 65, 0, -100 }, false });
        point.emplace_back(RelayPoint{ { 110, 0, -50 }, false });
        point.emplace_back(RelayPoint{ { 110, 0, 150 }, false });
        point.emplace_back(RelayPoint{ { 0, 0, 300 }, false });
        break;
    }
}

