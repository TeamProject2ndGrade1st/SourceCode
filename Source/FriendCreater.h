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

private:
    FriendManager* friendManager{ nullptr };
    GameObject* dragTarget{ nullptr };

    //¶¬‰Â”\‚È”ÍˆÍ
    RECT createRange{ -200,100,200,-100 };
    //‚»‚Ì”ÍˆÍ‚Ì’†S“_
    DirectX::XMFLOAT3 createPos{ 0,0,-100 };

    bool canCreate{ nullptr };
    DirectX::XMFLOAT4 color{1,0,0,1};

    Camera* camera{ nullptr };
};
