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


    FriendManager(Argent::Scene::BaseScene* currentScene);
    ~FriendManager() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void AddFriend(BaseFriend* _friend);
    
    

    BaseFriend* FindFriendComponentFromOwner(GameObject* wFriend) const;

    bool battleFlag{ false };

private:
    Argent::Scene::BaseScene* scene;
    std::vector<BaseFriend*> friendArray;
};

