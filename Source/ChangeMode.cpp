#include "ChangeMode.h"
#include "Character.h"

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

void ChangeMode::ChangeBattleMode()
{
    battleFlag = true;
    std::vector<GameObject*> _friend;
    std::vector<GameObject*> _enemy;

    GameObject::FindByTag(GameObject::Tag::Friend, _friend);
    GameObject::FindByTag(GameObject::Tag::Enemy, _enemy);

    for (auto& f : _friend)f->GetComponent<Character>()->SetActiveState(true);
    for (auto& e : _enemy)e->GetComponent<Character>()->SetActiveState(true);
}

void ChangeMode::ChangeReadyMode()
{
    battleFlag = false;
    std::vector<GameObject*> _friend;
    std::vector<GameObject*> _enemy;

    GameObject::FindByTag(GameObject::Tag::Friend, _friend);
    GameObject::FindByTag(GameObject::Tag::Enemy, _enemy);

    for (auto& f : _friend)f->GetComponent<Character>()->SetActiveState(false);
    for (auto& e : _enemy)e->GetComponent<Character>()->SetActiveState(false);

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
