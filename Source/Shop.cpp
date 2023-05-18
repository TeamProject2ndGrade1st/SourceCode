#include "Shop.h"


void Shop::Initialize()
{
    BaseComponent::Initialize();

    GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Shop.png"));

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

    /*SetItem();
    for (auto& item : items)
    {
        GetOwner()->AddComponent();
    }*/
}

void Shop::Begin()
{
}

void Shop::Update()
{
    if (easeEnd)
    {
        if (Argent::Input::GetKeyDown(KeyCode::B))
        {
            timer = 0;
            if (mode->openShop)mode->CloseShop();
            else mode->OpenShop();
            easeEnd = false;
            mode->canChange = false;
        }
    }
    

    if (mode->openShop)OpenUpdate();
    else CloseUpdate();

    
}

void Shop::OpenUpdate()
{
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
}

void Shop::CloseUpdate()
{
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
    items.emplace_back(new Item(ItemType::Creature, 100, 2.0f, { 270,720 }));
    items.emplace_back(new Item(ItemType::Drone, 20, 1.5f, { 478,676 }));
    items.emplace_back(new Item(ItemType::BloodAmo, 20, 1.5f, { 478,676 }));
    items.emplace_back(new Item(ItemType::ElectricAmo, 20, 1.5f, { 478,676 }));
    items.emplace_back(new Item(ItemType::BloodGrenade, 20, 1.5f, { 478,676 }));
    items.emplace_back(new Item(ItemType::ElectricGrenade, 20, 1.5f, { 478,676 }));
    items.emplace_back(new Item(ItemType::ChangeFight, 0, 1.5f, { 890,710 }));
    items.emplace_back(new Item(ItemType::ChangeEdit, 0, 1.5f, { 1150,710 }));
}

void Item::Update()
{
    auto mousePos = Argent::Input::Mouse::Instance().GetPosition();
    auto pos = GetOwner()->GetTransform()->GetPosition();

    float left = pos.x - button.left;
    float right = pos.x + button.right;
    float top = pos.y + button.top;
    float bottom = pos.y - button.bottom;

    if (left > mousePos.x)   return;
    if (right < mousePos.x)  return;
    if (top < mousePos.y)  return;
    if (bottom > mousePos.y)   return;

    if (Argent::Input::GetButtonDown(MouseButton::LeftButton))
    {
        if(pos.x < mousePos.x)Buy();
        else Sale();
    }
}

void Item::Initialize()
{
    GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/ShopButton.png"));
    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(initPos.x, initPos.y, 0));
}

void Item::Buy()
{
}

void Item::Sale()
{
}
