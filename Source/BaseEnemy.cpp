#include "BaseEnemy.h"
#include "EnemyStateDerivad.h"

int BaseEnemy::num = 0;

// コンストラクタ
BaseEnemy::BaseEnemy(const char* name, DirectX::XMFLOAT3 pos) :
    Character(name, pos)
{
    num++;
}

void BaseEnemy::Initialize()
{
    Character::Initialize();

    if (!fManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::FriendManager, g);
        if (g.size() > 0)
            fManager = g.at(0)->GetComponent<FriendManager>();
    }
    //GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    
    
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = {
    //    attackAreaRadius 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f
    //};
    

    GetOwner()->ReplaceTag(GameObject::Tag::Enemy);
}

void BaseEnemy::Begin()
{
    if (!fManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::FriendManager, g);
        if (g.size() > 0)
            fManager = g.at(0)->GetComponent<FriendManager>();
    }
}

void BaseEnemy::Update()
{
    if (!FriendManager::battleFlag)return;

    stateMachine.get()->Update();

    
}

void BaseEnemy::DrawDebug()
{
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::TreePop();
    }
}



BaseFriend* BaseEnemy::SearchFriend1()
{
    // TODO: return ステートメントをここに挿入します
        // Friendタグが付いているGameObjectを探す
    std::vector<GameObject*> Friend;
    GameObject::FindByTag(GameObject::Tag::Friend, Friend);

    DirectX::XMFLOAT3 pos = GetOwner()->GetTransform()->GetPosition();
    DirectX::XMFLOAT4 angle = GetOwner()->GetTransform()->GetRotation();

    // TO DO 途中
    for (auto f : Friend)
    {
        // Friend の positionを取る
        DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

        float vx = pos.x - friendPos.x;
        float vy = pos.y - friendPos.y;
        float vz = pos.z - friendPos.z;
        float dist = sqrtf(vx * vx + vz * vz);

        if (dist < 50)
        {
            float distXZ = sqrtf(vx * vx + vz * vz);
            // 単位ベクトル化
            vx /= distXZ;
            vz /= distXZ;

            // 方向ベクトル化
            float frontX = sinf(angle.y);
            float frontZ = cosf(angle.y);
            // 2つのベクトルの内積値で前後判定
            float dot = (frontX * vx) + (frontZ * vz);
            if (dot > 0.0f)
            {
                f->GetTransform()->SetPosition(friendPos);
                auto* ret = fManager->FindFriendComponentFromOwner(f);
                return ret;
                //return f;
            }
        }
    }
    return nullptr;
}
BaseFriend* BaseEnemy::SearchFriend()
{
    // TODO: return ステートメントをここに挿入します
        // Friendタグが付いているGameObjectを探す
    std::vector<GameObject*> Friend;
    GameObject::FindByTag(GameObject::Tag::Friend, Friend);

    DirectX::XMFLOAT3 pos = GetOwner()->GetTransform()->GetPosition();
    DirectX::XMFLOAT4 angle = GetOwner()->GetTransform()->GetRotation();

    // TO DO 途中


    for (auto f : Friend)
    {
        if (!f->GetIsActive())continue;

         //Friend の positionを取る
        DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

        float vx = pos.x - friendPos.x;
        float vy = pos.y - friendPos.y;
        float vz = pos.z - friendPos.z;
        float dist = sqrtf(vx * vx + vz * vz);

        //if (dist < 50)
        //{
        float distXZ = sqrtf(vx * vx + vz * vz);
        //単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;

        //方向ベクトル化
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //2つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            f->GetTransform()->SetPosition(friendPos);
            auto* ret = fManager->FindFriendComponentFromOwner(f);
            return ret;
            //return f;
        }
        //}
    }
    return nullptr;
}
