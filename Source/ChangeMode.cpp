#include "ChangeMode.h"
#include "Character.h"
#include "FriendManager.h"
#include "EnemyManager.h"
#include "Shop.h"

#include <vector>

void ChangeMode::Initialize()
{
    BaseComponent::Initialize();

    ChangeReadyMode();

    GetOwner()->ReplaceTag(GameObject::Tag::ChangeMode);
}

void ChangeMode::Update()
{
    if (battleFlag)
    {
        openShop = false;
    }

    /*if (Argent::Input::GetKeyDown(KeyCode::F))
    {
        if (currentMode == ChangeMode::Mode::Ready)
        {
            ChangeEditMode();
        }
        else if (currentMode == ChangeMode::Mode::Edit)
        {
            ChangeReadyMode();
        }
    }

    if (Argent::Input::GetKeyDown(KeyCode::R))
    {
        ChangeBattleMode();
    }*/

    static bool endFlag;
    if (!endFlag)
    {
        if (currentMode == Mode::Battle)
        {
            if (BaseEnemy::num <= 0 || BaseFriend::num <= 0)
            {
                endFlag = true;
                auto* g = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/giveup.png");
                GameObject::Instantiate("giveup",g);
                g->GetOwner()->GetTransform()->SetPosition(
                    DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth()/2, Argent::Graphics::GetWindowHeight() / 2,0));
                g->SetTexPos(DirectX::XMFLOAT2(214,18));
            }
        }
    }
    
}

void ChangeMode::DrawDebug()
{
    ImGui::Text("CHECKBOX OSUNA! KAKUNINYOU DESU");
    ImGui::Checkbox("BattleFlag", &battleFlag);
    if(ImGui::Button("ChangeBattleMode"))ChangeBattleMode();
    if(ImGui::Button("ChangeReadyMode"))ChangeReadyMode();
    if(ImGui::Button("ChangeEditMode"))ChangeEditMode();
}

void ChangeMode::ChangeBattleMode()
{
    if (!canChange)return;
    currentMode = Mode::Battle;
    GameObject::FindByName("Camera")->GetComponent<Camera>()->UseSceneCamera();
    battleFlag = true;
    std::vector<GameObject*> obj;
    //std::vector<GameObject*> _enemy;

    EnemyManager::battleFlag = true;
    FriendManager::battleFlag = true;

    if (openShop)
    {
        openShop = false;
        obj.clear();
        GameObject::FindByTag(GameObject::Tag::Shop, obj);
        obj.at(0)->GetComponent<Shop>()->easeEnd = false;
        GameObject::Destroy(obj.at(0));
    }
}

void ChangeMode::ChangeReadyMode()
{
    if (!canChange)return;
    currentMode = Mode::Ready;
    GameObject::FindByName("Camera")->GetComponent<Camera>()->UseSceneCamera();
    battleFlag = false;
    
    //std::vector<GameObject*> _enemy;
    EnemyManager::battleFlag = false;
    FriendManager::battleFlag = false;
   

}

void ChangeMode::ChangeEditMode()
{
    if (!canChange)return;
    currentMode = Mode::Edit;

    GameObject::FindByName("SecondCamera")->GetComponent<Camera>()->UseSceneCamera();
    battleFlag = false;
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
