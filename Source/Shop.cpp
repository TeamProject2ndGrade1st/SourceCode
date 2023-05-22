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
}

void Shop::Begin()
{
    //マウスカーソル座標更新
    auto disMousePos = Argent::Input::Mouse::Instance().GetPosition();
    disMousePos.x -= static_cast<float>(width) / 2;
    disMousePos.y -= static_cast<float>(height) / 2;

    mousePos.x += disMousePos.x;
    mousePos.y += disMousePos.y;

    if (mousePos.x > Argent::Graphics::GetWindowWidth())mousePos.x = Argent::Graphics::GetWindowWidth();
    if (mousePos.x < 0)mousePos.x = 0;
    if (mousePos.y > Argent::Graphics::GetWindowHeight())mousePos.y = Argent::Graphics::GetWindowHeight();
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
            timer = 0;
            if (mode->openShop)mode->CloseShop();
            else mode->OpenShop();
            easeEnd = false;
            mode->canChange = false;
            mouse->GetTransform()->SetPosition(DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth(),Argent::Graphics::GetWindowHeight(), 0.0f));
        }
    }
    

    if (mode->openShop)OpenUpdate();
    else CloseUpdate();

    
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

    if (mode->battleFlag)return;
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

void Shop::SetItem()
{
    items.emplace_back(new GameObject("Item Creature",new Item(ItemType::Creature, 100, 2.0f, { 220,645 })));
    items.emplace_back(new GameObject("Item Drone",new Item(ItemType::Drone, 20, 1.5f, { 478,645 })));
    items.emplace_back(new GameObject("Item BloodAmo",new Item(ItemType::BloodAmo, 20, 1.5f, { 700,380 })));
    items.emplace_back(new GameObject("Item ElectricAmo",new Item(ItemType::ElectricAmo, 20, 1.5f, { 875,380 })));
    items.emplace_back(new GameObject("Item BloodGre",new Item(ItemType::BloodGrenade, 20, 1.5f, { 1210,475 })));
    items.emplace_back(new GameObject("Item ElectricGre",new Item(ItemType::ElectricGrenade, 20, 1.5f, { 1053,475 })));
    items.emplace_back(new GameObject("Item ChangeFight",new Item(ItemType::ChangeFight, 0, 1.5f, { 890,680 })));
    items.emplace_back(new GameObject("Item ChangeEdit",new Item(ItemType::ChangeEdit, 0, 1.5f, { 1150,680 })));

    for (auto& item : items)
    {
        item->GetComponent<Item>()->shop = this;
        GetOwner()->AddChild(item);
    }
}

void Item::Update()
{
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
        if(pos.y > shop->mousePos.y)Buy();
        else Sale();
    }
}

void Item::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/ShopButton.png"));
    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(initPos.x, initPos.y, 0));
}

void Item::Buy()
{
}

void Item::Sale()
{
}
