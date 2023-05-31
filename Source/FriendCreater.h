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
    void DrawDebug()override;

    void SetFriendByClick();
    void ImagineFriendUpdate();

    bool DragFriend();

    void CanCreate(DirectX::XMFLOAT3 pos);

    int canCreateNumber[2]{ 0,0 };

private:
    FriendManager* friendManager{ nullptr };
    GameObject* dragTarget{ nullptr };

    //¶¬‰Â”\‚È”ÍˆÍ
    RECT createRange{ -200,150,200,-150 };
    //‚»‚Ì”ÍˆÍ‚Ì’†S“_
    DirectX::XMFLOAT3 createPos{ 0,0,-200 };

    bool canCreate{ nullptr };
    bool canDrag{ nullptr };
    DirectX::XMFLOAT4 color{1,0,0,1};

    Camera* camera{ nullptr };

    int createType{static_cast<int>(FriendManager::Type::Creature)};

    GameObject* imageFriend[2];
};
