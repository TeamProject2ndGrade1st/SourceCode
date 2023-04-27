#include "FriendManager.h"
#include "FriendCreature.h"
#include "FriendDrone.h"

void FriendManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::FriendManager);
}

void FriendManager::Update()
{
    for (auto activer = friendArray.begin();activer != friendArray.end();++activer)
    {
        for (auto passiver = activer + 1; passiver != friendArray.end(); ++passiver)
        {
            (*activer)->OnCollision((*passiver)->GetOwner()->GetComponent<Argent::Component::Collider::SphereCollider>());
        }
    }
}

void FriendManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    static int type{};
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -1000.0f, 1000.0f);
        ImGui::SliderInt("FriendType", &type, 0, static_cast<int>(Type::End)-1);
        ImGui::Text("1: Creature\n2: Drone\n");
        if (ImGui::Button("AddFriend"))
        {
            switch (type)
            {
            case static_cast<int>(Type::Creature)://クリーチャー
                AddFriend(new FriendCreature(pos));
                break;
            case static_cast<int>(Type::Drone):
                AddFriend(new FriendDrone(pos));
                break;
            }
        }
        ImGui::TreePop();
    }
    BaseActor::DrawDebug();
}

void FriendManager::AddFriend(BaseFriend* _friend)
{
    GameObject::Instantiate("Friend", _friend);
    /*scene->AddObject(new GameObject(
        "Friend", 
        _friend));*/
    friendArray.emplace_back(_friend);

    //タグ登録はそれぞれのフレンド本体で行っている
}


BaseFriend* FriendManager::FindFriendComponentFromOwner(GameObject* wFriend) const
{
    for (auto& f : friendArray)
    {
        if (!f) continue;
        if (wFriend == f->GetOwner())
            return f;
    }

    return nullptr;
}

