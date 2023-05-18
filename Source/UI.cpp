#include "UI.h"

void UI::Initialize()
{
    GameObject::Instantiate("Reticle", reticle = new Reticle("./Resources/Image/ReticleYellow.png"));

    GetOwner()->AddComponent(huel       = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/UI_fuel.png"));
    GetOwner()->AddComponent(base       = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_1.png"));
    GetOwner()->AddComponent(hp         = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_2.png"));
    GetOwner()->AddComponent(lifeGure   = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_5.png"));
    GetOwner()->AddComponent(elecGure   = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_6.png"));
    GetOwner()->AddComponent(lifeAmo    = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_7.png"));
    GetOwner()->AddComponent(elecAmo    = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_8.png"));
    elecGure->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    elecAmo->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    huel->SetOffset(DirectX::XMFLOAT3(1183.85f, 48.45f, 0));
    huel->SetScale(DirectX::XMFLOAT3(0.96f, 0.99f, 1));
    huel->SetTexPos(DirectX::XMFLOAT2(0, -463.2f));

    std::vector<GameObject*> obj;
    GameObject::FindByTag(GameObject::Tag::Player,obj);
    player = obj.at(0)->GetComponent<Player>();
    obj.clear();

    gun = player->GetGun();


    GameObject::FindByTag(GameObject::Tag::Shop,obj);
    shop = obj.at(0)->GetComponent<Shop>();
    obj.clear();

    GameObject::FindByTag(GameObject::Tag::ChangeMode, obj);
    mode = obj.at(0)->GetComponent<ChangeMode>();
    obj.clear();
}

void UI::Update()
{
    if (mode->openShop || !shop->easeEnd)
    {
        float posX = shop->GetOwner()->GetTransform()->GetPosition().x;
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(posX - Argent::Graphics::GetWindowWidth(), 0, 0));
    }
    else GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
    

    auto scale = huel->GetScale();
    scale.y = -player->FuelRatio() + 0.01f;
    if (scale.y >= 0)
    {
        scale.y = 0;
    }
    huel->SetScale(DirectX::XMFLOAT3(scale.x, scale.y, scale.z));

    if (gun->GetMode() == Bullet::Mode::Creature)
    {
        lifeAmo->SetScale(DirectX::XMFLOAT3(1, 1, 1));
        elecAmo->SetScale(DirectX::XMFLOAT3(0, 0, 0));
        auto* material = reticle->GetOwner()->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial();
        material->color = Argent::Color(DirectX::XMFLOAT4(1,0,0,1));
    }
    else if (gun->GetMode() == Bullet::Mode::Machine)
    {
        lifeAmo->SetScale(DirectX::XMFLOAT3(0, 0, 0));
        elecAmo->SetScale(DirectX::XMFLOAT3(1, 1, 1));
        auto* material = reticle->GetOwner()->GetComponent<Argent::Component::Renderer::SpriteRenderer>()->GetMaterial();
        material->color = Argent::Color(DirectX::XMFLOAT4(1, 1, 1, 1));
    }

    DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth()/2, Argent::Graphics::GetWindowHeight()/2,0);
    pos.x += GetOwner()->GetTransform()->GetPosition().x;
    reticle->GetOwner()->GetTransform()->SetPosition(pos);
}
