#include "FriendManager.h"
#include "FriendCreature.h"

void FriendManager::Initialize()
{
    BaseComponent::Initialize();
}

void FriendManager::Update()
{
    
}

void FriendManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    if (ImGui::TreeNode(GetName().c_str()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -100.0f, 100.0f);
        if (ImGui::Button("AddFriend"))
        {
            AddFriend(new FriendCreature(pos));
        }
        ImGui::TreePop();
    }
    BaseComponent::DrawDebug();
}

void FriendManager::AddFriend(BaseFriend* _friend)
{
    GameObject::Instantiate("Friend", _friend);
    /*scene->AddObject(new GameObject(
        "Friend", 
        _friend));*/
    friendArray.emplace_back(std::move(_friend));

    //タグ登録はそれぞれのフレンド本体で行っている
}

std::unique_ptr<BaseFriend> FriendManager::FindByTag(Tag tag, std::vector<std::unique_ptr<BaseFriend>>& array)
{

    return std::unique_ptr<BaseFriend>();
}
