#include "ChangeMode.h"
#include "Character.h"
#include "FriendManager.h"

#include <vector>

void ChangeMode::Initialize()
{
    BaseComponent::Initialize();

    ChangeReadyMode();

    GetOwner()->ReplaceTag(GameObject::Tag::ChangeMode);
}

void ChangeMode::Update()
{
}

void ChangeMode::DrawDebug()
{
    ImGui::Text("CHECKBOX OSUNA! KAKUNINYOU DESU");
    ImGui::Checkbox("BattleFlag", &battleFlag);
    if(ImGui::Button("ChangeBattleMode"))ChangeBattleMode();
    if(ImGui::Button("ChangeReadyMode"))ChangeReadyMode();
}

void ChangeMode::ChangeBattleMode()
{
    battleFlag = true;
    std::vector<GameObject*> friendM;
    //std::vector<GameObject*> _enemy;

    GameObject::FindByTag(GameObject::Tag::FriendManager, friendM);
    friendM.at(0)->GetComponent<FriendManager>()->battleFlag = true;

}

void ChangeMode::ChangeReadyMode()
{
    battleFlag = false;
    std::vector<GameObject*> friendM;
    //std::vector<GameObject*> _enemy;

    GameObject::FindByTag(GameObject::Tag::FriendManager, friendM);
    friendM.at(0)->GetComponent<FriendManager>()->battleFlag = false;

}

bool ChangeMode::OpenShop()
{
    //êÌì¨íÜÇÕÇ–ÇÁÇØÇ»Ç¢
    return battleFlag ? openShop = false : openShop = true;
}

void ChangeMode::CloseShop()
{
    openShop = false;
}
