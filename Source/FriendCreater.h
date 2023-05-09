#pragma once
#include "Argent/Argent.h"
#include "FriendManager.h"

class FriendCreater : public Argent::Component::BaseActor
{
public:
    FriendCreater() : BaseActor("FriendCreater") {}
    ~FriendCreater() {};

    void Initialize()override;
    void Update()override;

    void SetFriendByClick();
    void ImagineFriendUpdate();

    bool DragFriend();

private:
    FriendManager* friendManager{ nullptr };
    GameObject* dragTarget{ nullptr };
};

