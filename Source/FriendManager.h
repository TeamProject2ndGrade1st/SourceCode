#pragma once
#include "Argent/Scene/BaseScene.h"

class BaseFriend;

class FriendManager : public Argent::Component::BaseActor
{
public:
    //�����̃^�C�v�ɍ��킹�ă^�O�t������
    enum class Tag : unsigned int
    {
        UnTagged    = 0x01,
        Creature    = 0x01 << 1,
        Drone       = 0x01 << 2
    };

    enum class Type
    {
        Creature,
        Drone,
        End,
    };


    FriendManager(Argent::Scene::BaseScene* currentScene) : 
        BaseActor("FriendManager"), scene(currentScene){}
    ~FriendManager() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddFriend(BaseFriend* _friend);
    BaseFriend* FindByTag(Tag tag, std::vector<BaseFriend*>& array);

private:
    std::vector<BaseFriend*> friendArray;
    Argent::Scene::BaseScene* scene;
};

