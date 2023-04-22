#pragma once
#include "Argent/Scene/BaseScene.h"

class BaseFriend;

class FriendManager : public Argent::Component::BaseComponent
{
public:
    //味方のタイプに合わせてタグ付けする
    enum class Tag : unsigned int
    {
        UnTagged    = 0x00,
        Creature    = 0x01 << 0,
        Drone       = 0x01 << 1
    };


    FriendManager(Argent::Scene::BaseScene* currentScene) : 
        BaseComponent("FriendManager"), scene(currentScene){}
    ~FriendManager() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddFriend(BaseFriend* _friend);
    std::unique_ptr<BaseFriend> FindByTag(Tag tag,std::vector<std::unique_ptr<BaseFriend>>& array);

private:
    std::vector<std::unique_ptr<BaseFriend>> friendArray;
    Argent::Scene::BaseScene* scene;
};

