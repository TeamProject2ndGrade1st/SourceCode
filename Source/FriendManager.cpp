#include "FriendManager.h"
#include "FriendCreature.h"
#include "FriendDrone.h"
#include "Argent/Graphic/Graphics.h"

void FriendManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::FriendManager);
}

void FriendManager::Update()
{
#if _DEBUG
    static DirectX::XMFLOAT3 pos{};
    pos.z = -440;
    if (Argent::Input::GetKeyUp(KeyCode::E))
    {
        AddFriend(new FriendCreature(pos));
    }

#endif // _DEBUG

    //実際のフレンド配列と同期させる
    std::vector<GameObject*> f;
    GameObject::FindByTag(GameObject::Tag::Friend, f);
    if (friendArray.size() == f.size())
    {
        std::vector<std::vector<BaseFriend*>::iterator> destroyArray;
        for (auto it = friendArray.begin(); it != friendArray.end(); ++it)
        {
            bool destroyFlag{ true };
            for (auto& fr : f)
            {
                if (fr == (*it)->GetOwner())
                {
                    destroyFlag = false;
                    break;
                }
            }
            if (destroyFlag)destroyArray.emplace_back(it);
        }
        if (destroyArray.size() > 0)
        {
            for (auto& d : destroyArray)
            {
                friendArray.erase(d);
            }
        }
    }
   
    //味方同士の判定   
    for (auto activer = f.begin();activer != f.end();++activer)
    {
        for (auto passiver = activer + 1; passiver != f.end(); ++passiver)
        {
            if (!(*passiver)->GetComponent<Argent::Component::Collider::SphereCollider>())continue;
            (*activer)->GetComponent<Argent::Component::Collider::SphereCollider>()->CollisionDetection((*passiver)->GetComponent<Argent::Component::Collider::SphereCollider>());
        }
    }
}

void FriendManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    static int type{};
    static int route{};
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -1000.0f, 1000.0f);
        ImGui::SliderInt("FriendType", &type, 0, static_cast<int>(Type::End)-1);
        ImGui::Text("1: Creature\n2: Drone\n");
        ImGui::SliderInt("Route", &route, 0, 2);
        ImGui::Text("Left:0,Center:1,Right:2");
        if (ImGui::Button("AddFriend"))
        {
            switch (type)
            {
            case static_cast<int>(Type::Creature)://クリーチャー
                AddFriend(new FriendCreature(pos,route));
                break;
            case static_cast<int>(Type::Drone):
                AddFriend(new FriendDrone(pos,route));
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

    //タグ登録はそれぞれのフレンド本体で行っている
    friendArray.emplace_back(_friend);

    //これまでに消されてしまったフレンドの分配列を調整する
    
    
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