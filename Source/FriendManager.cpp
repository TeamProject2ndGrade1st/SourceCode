#include "FriendManager.h"
#include "FriendCreature.h"
#include "FriendDrone.h"

void FriendManager::Initialize()
{
    BaseActor::Initialize();
}

void FriendManager::Update()
{
    
}

void FriendManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    static int type{};
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -100.0f, 100.0f);
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

BaseFriend* FriendManager::FindByTag(Tag tag, std::vector<BaseFriend*>& array)
{
    return nullptr;
    //return std::unique_ptr<BaseFriend>();
}
