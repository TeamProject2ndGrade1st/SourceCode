#include "Shop.h"


void Shop::Initialize()
{
    BaseComponent::Initialize();

    GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Shop.png"));

    mouse = new GameObject("ShopMouse",new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/mouse.png"));
    GetOwner()->AddChild(mouse);
    DirectX::XMFLOAT4& mouseColor = mouse->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial()->color.color;
    mouseColor = DirectX::XMFLOAT4(1,0,0,1);
   

    GetOwner()->ReplaceTag(GameObject::Tag::Shop);

    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth(), 0, 0));

    easeFunc = &Easing::OutBounce;

    std::vector<GameObject*> p;
    GameObject::FindByTag(GameObject::Tag::Player, p);
    player = p.at(0)->GetComponent<Player>();

#ifdef _DEBUG
    currentEase = "OutBounce";
    std::string s = "OutExp"; easeFunctoins.push_back(s);
    s = "OutSine";easeFunctoins.push_back(s);
    s = "OutQuad";easeFunctoins.push_back(s);
    s = "OutCubic";easeFunctoins.push_back(s);
    s = "OutQuart";easeFunctoins.push_back(s);
    s = "OutBounce";easeFunctoins.push_back(s);
    s = "InExp"; easeFunctoins.push_back(s);
    s = "InSine"; easeFunctoins.push_back(s);
    s = "InQuad"; easeFunctoins.push_back(s);
    s = "InCubic"; easeFunctoins.push_back(s);
    s = "InQuart"; easeFunctoins.push_back(s);
    s = "InBounce"; easeFunctoins.push_back(s);

    easeFuncs.push_back(&Easing::OutExp);
    easeFuncs.push_back(&Easing::OutSine);
    easeFuncs.push_back(&Easing::OutQuad);
    easeFuncs.push_back(&Easing::OutCubic);
    easeFuncs.push_back(&Easing::OutQuart);
    easeFuncs.push_back(&Easing::OutBounce);
    easeFuncs.push_back(&Easing::InExp);
    easeFuncs.push_back(&Easing::InSine);
    easeFuncs.push_back(&Easing::InQuad);
    easeFuncs.push_back(&Easing::InCubic);
    easeFuncs.push_back(&Easing::InQuart);
    easeFuncs.push_back(&Easing::InBounce);
#endif // _DEBUG

    std::vector<GameObject*> m;
    GameObject::FindByTag(GameObject::Tag::ChangeMode, m);
    mode = m.at(0)->GetComponent<ChangeMode>();

    SetItem();

    std::vector<GameObject*> fCreater;
    GameObject::FindByTag(GameObject::Tag::FriendCreatar, fCreater);
    friendCreater = fCreater.at(0)->GetComponent<FriendCreater>();

   
    //auto* Money = GameObject::Instantiate("Money", money = new Number(&player->moneyInPoss, 4));
    auto* Money = new GameObject("Money", money = new Number(&player->moneyInPoss, 5));
    GetOwner()->AddChild(Money);
    
    //GetOwner()->AddComponent(money = new Number(&player->moneyInPoss, 4));
}

void Shop::Begin()
{
    //イニシャライザに入れても動かんかったからしゃーなしこっち
    auto posU = GetOwner()->GetTransform()->GetPosition();
    money->GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(1180 + posU.x, 555 + posU.y, 0));
    money->GetOwner()->GetTransform()->SetScale(DirectX::XMFLOAT3(1.5f, 1.5f, 1));

    //マウスカーソル座標更新
    auto disMousePos = Argent::Input::Mouse::Instance().GetPosition();
    disMousePos.x -= static_cast<float>(width) / 2;
    disMousePos.y -= static_cast<float>(height) / 2;

    mousePos.x += disMousePos.x;
    mousePos.y += disMousePos.y;

    if (mousePos.x > Argent::Graphics::GetWindowWidth()-40)mousePos.x = Argent::Graphics::GetWindowWidth()-40;
    if (mousePos.x < 0)mousePos.x = 0;
    if (mousePos.y > Argent::Graphics::GetWindowHeight()-40)mousePos.y = Argent::Graphics::GetWindowHeight()-40;
    if (mousePos.y < 0)mousePos.y = 0;

    mouse->GetTransform()->SetPosition(DirectX::XMFLOAT3(mousePos.x,mousePos.y,0));
    mouse->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));
    mouse->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->SetTexPos(DirectX::XMFLOAT2(0,0));
}

void Shop::Update()
{
    if (easeEnd && !mode->battleFlag)
    {
        if (Argent::Input::GetKeyDown(KeyCode::B))
        {
            if (mode->openShop)CloseShop();
            else OpenShop();
        }
    }
    
    std::vector<GameObject*> p;
    GameObject::FindByTag(GameObject::Tag::Player, p);
    if (mode->openShop)
    {
        OpenUpdate();
        p.at(0)->GetComponent<Player>()->use = false;
    }
    else
    {
        CloseUpdate();
        p.at(0)->GetComponent<Player>()->use = true;
    }

    
}

void Shop::OpenUpdate()
{
    DirectX::XMFLOAT4& mouseColor = mouse->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial()->color.color;
    mouseColor.w = 1;

    if (!easeEnd)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(
            (*easeFunc)(timer, easeMaxTime, 0.0f, Argent::Graphics::GetWindowWidth()),
            0, 0)
        );
        if (timer < easeMaxTime)timer += Argent::Timer::GetDeltaTime();
        else
        {
            GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
            easeEnd = true;
            mode->canChange = true;
        }
    }

    for (auto& item : items)
    {
        item->Update();
    }
}

void Shop::CloseUpdate()
{
    DirectX::XMFLOAT4& mouseColor = mouse->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial()->color.color;
    mouseColor.w = 0;

    //if (mode->battleFlag)return;
    if (!easeEnd)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(
            (*easeFunc)(timer, easeMaxTime, Argent::Graphics::GetWindowWidth(), 0.0f),
            0, 0)
        );
        if (timer < easeMaxTime)timer += Argent::Timer::GetDeltaTime();
        else 
        { 
            GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, Argent::Graphics::GetWindowWidth(), 0));
            easeEnd = true;
            mode->canChange = true;
        }
    }
}

void Shop::DrawDebug()
{
    ImGui::SliderFloat("Timer", &timer,0,easeMaxTime);
    ImGui::DragFloat("MaxTimer", &easeMaxTime,0.01f);

#ifdef _DEBUG
    ImGuiComboUI("Easing",currentEase,easeFunctoins);
#endif // _DEBUG
    
}

void Shop::OpenShop()
{
    timer = 0;
    mode->OpenShop();
    easeEnd = false;
    mode->canChange = false;
    mouse->GetTransform()->SetPosition(DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth(), Argent::Graphics::GetWindowHeight(), 0.0f));
}

void Shop::CloseShop()
{
    timer = 0;
    mode->CloseShop();
    easeEnd = false;
    mode->canChange = false;
    mouse->GetTransform()->SetPosition(DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth(), Argent::Graphics::GetWindowHeight(), 0.0f));
}

void Shop::SetItem()
{
    items.emplace_back(new GameObject("Item Creature", new ItemCreature(ItemType::Creature, 100, 2.0f, { 188,613 }, {-66,34,66,-34})));
    items.emplace_back(new GameObject("Item Drone",new ItemDrone(ItemType::Drone, 20, 1.5f, { 450,613 }, { -66,34,66,-34 })));
    items.emplace_back(new GameObject("Item BloodAmo", new ItemBloodAmo(ItemType::BloodAmo, 20, 1.5f, { 668,348 }, {-45,19,45,-19})));
    items.emplace_back(new GameObject("Item ElectricAmo",new ItemElectricAmo(ItemType::ElectricAmo, 20, 1.5f, { 843,348 }, { -45,19,45,-19 })));
    items.emplace_back(new GameObject("Item BloodGre",new ItemBloodGrenade(ItemType::BloodGrenade, 20, 1.5f, { 1181,443 }, { -40,37,40,-37 })));
    items.emplace_back(new GameObject("Item ElectricGre",new ItemElectricGrenade(ItemType::ElectricGrenade, 20, 1.5f, { 1021,443 }, { -40,37,40,-37 })));
    items.emplace_back(new GameObject("Item ChangeBattle",new ItemChangeBattle(ItemType::ChangeBattle, 0, 1.5f, { 858,648 },{-122,32,122,-32})));
    items.emplace_back(new GameObject("Item ChangeEdit",new ItemChangeEdit(ItemType::ChangeEdit, 0, 1.5f, { 1118,648 }, { -122,32,122,-32 })));

    for (auto& item : items)
    {
        GetOwner()->AddChild(item);
    }
}

void Item::Update()
{
    if (timer > 0)
    {
        timer -= Argent::Timer::GetDeltaTime(); return;
    }

    if (shop == nullptr)
    {
        std::vector<GameObject*> s;
        GameObject::FindByTag(GameObject::Tag::Shop, s);
        shop = s.at(0)->GetComponent<Shop>();
    }

    if (!shop->mode->openShop)
    {
        return;
    }

    auto pos = GetOwner()->GetTransform()->GetPosition();

    float left = pos.x + button.left;
    float right = pos.x + button.right;
    float top = pos.y + button.top;
    float bottom = pos.y + button.bottom;

    if (left > shop->mousePos.x)   return;
    if (right < shop->mousePos.x)  return;
    if (top < shop->mousePos.y)  return;
    if (bottom > shop->mousePos.y)   return;

    shop->mouse->GetTransform()->SetScale(DirectX::XMFLOAT3(2, 2, 1));
    shop->mouse->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->SetTexPos(DirectX::XMFLOAT2(32.0f, 32.0f));

    if (Argent::Input::GetButtonDown(MouseButton::LeftButton))
    {
        if (pos.y > shop->mousePos.y)
        {
            Buy();
            timer = 0.1f;
        }
        else
        {
            Sale();
            timer = 0.1f;
        }
    }
}

void Item::Initialize()
{
    BaseActor::Initialize();
    /*GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/ShopButton.png"));
    GetOwner()->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial()->color.color = DirectX::XMFLOAT4(1, 1, 1, 0.5f);*/
    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(initPos.x, initPos.y, 0));

    DirectX::XMFLOAT2 scale = {
        (static_cast<float>(button.right) - static_cast<float>(button.left)) / 64,
        (static_cast<float>(button.top) - static_cast<float>(button.bottom)) / 64
    };
    GetOwner()->GetTransform()->SetScale(DirectX::XMFLOAT3(scale.x, scale.y, 1));
}

void Item::Buy()
{
}

void Item::Sale()
{
    //この関数をオーバーロードしないクラスは売却操作がないので購入と同じ処理をする
    Buy();
}

void ItemCreature::Buy()
{
    shop->friendCreater->canCreateNumber[static_cast<int>(FriendManager::Type::Creature)]++;
}

void ItemCreature::Sale()
{
    shop->friendCreater->canCreateNumber[static_cast<int>(FriendManager::Type::Creature)]--;
}

void ItemDrone::Buy()
{
    shop->friendCreater->canCreateNumber[static_cast<int>(FriendManager::Type::Drone)]++;
}

void ItemDrone::Sale()
{
    shop->friendCreater->canCreateNumber[static_cast<int>(FriendManager::Type::Drone)]--;
}

void ItemChangeEdit::Buy()
{
    shop->mode->ChangeEditMode();
    shop->CloseShop();
}

void ItemChangeBattle::Buy()
{
    shop->mode->ChangeBattleMode();
}

void ItemBloodAmo::Buy()
{
}

void ItemBloodAmo::Sale()
{
}

void ItemElectricAmo::Buy()
{
}

void ItemElectricAmo::Sale()
{
}

void ItemBloodGrenade::Buy()
{
}

void ItemBloodGrenade::Sale()
{
}

void ItemElectricGrenade::Buy()
{
}

void ItemElectricGrenade::Sale()
{
}
