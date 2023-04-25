#pragma once
#include "Argent/Scene/BaseScene.h"

class BaseFriend;

class FriendManager : public Argent::Component::BaseActor
{
public:
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

    BaseFriend* FindFriendOwner(GameObject* wFriend) const;
private:
    std::vector<BaseFriend*> friendArray;
    Argent::Scene::BaseScene* scene;
};

