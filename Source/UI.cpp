#include "UI.h"
#include "Grenade.h"

void UI::Initialize()
{
    GameObject::Instantiate("Reticle", reticle = new Reticle("./Resources/Image/ReticleYellow.png"));

    GetOwner()->AddComponent(huel = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/UI_fuel.png"));
    GetOwner()->AddComponent(base = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_1.png"));
    GetOwner()->AddComponent(hp = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_2.png"));
    GetOwner()->AddComponent(lifeGre = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_5.png"));
    GetOwner()->AddComponent(elecGre = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_6.png"));
    GetOwner()->AddComponent(lifeAmo = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_7.png"));
    GetOwner()->AddComponent(elecAmo = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/sentou_0516_8.png"));

    GetOwner()->AddComponent(edit = new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/EditMode.png"));
    edit->GetMaterial()->color.color = { 1,1,1,0 };

    elecGre->SetScale(DirectX::XMFLOAT3(0, 0, 0));
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

    //�O���l�[�h�ݒ�
    if (player->grenadeMode == Grenade::Mode::Creature)
    {
        lifeGre->SetScale(DirectX::XMFLOAT3(1, 1, 1));
        elecGre->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    }
    else if (player->grenadeMode == Grenade::Mode::Machine)
    {
        lifeGre->SetScale(DirectX::XMFLOAT3(0, 0, 0));
        elecGre->SetScale(DirectX::XMFLOAT3(1, 1, 1));
    }

    DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth()/2, Argent::Graphics::GetWindowHeight()/2,0);
    pos.x += GetOwner()->GetTransform()->GetPosition().x;
    reticle->GetOwner()->GetTransform()->SetPosition(pos);

    if (mode->currentMode == ChangeMode::Mode::Edit)
    {
        edit->GetMaterial()->color.color = { 1,1,1,1 };
        base->GetMaterial()->color.color = { 1,1,1,0 };
        hp->GetMaterial()->color.color = { 1,1,1,0 };
        lifeGre->GetMaterial()->color.color = { 1,1,1,0 };
        elecGre->GetMaterial()->color.color = { 1,1,1,0 };
        lifeAmo->GetMaterial()->color.color = { 1,1,1,0 };
        elecAmo->GetMaterial()->color.color = { 1,1,1,0 };
        huel->GetMaterial()->color.color = { 1,1,1,0 };
    }
    else
    {
        edit->GetMaterial()->color.color = { 1,1,1,0 };
        base->GetMaterial()->color.color = { 1,1,1,1 };
        hp->GetMaterial()->color.color = { 1,1,1,1 };
        lifeGre->GetMaterial()->color.color = { 1,1,1,1 };
        elecGre->GetMaterial()->color.color = { 1,1,1,1 };
        lifeAmo->GetMaterial()->color.color = { 1,1,1,1 };
        elecAmo->GetMaterial()->color.color = { 1,1,1,1 };
        huel->GetMaterial()->color.color = { 1,1,1,1 };
    }

    
}


//�����t�H���g�̏���

void OneDigit::Initialize()
{
    numbers.emplace_back(GameObject("Num 0", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 1", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 2", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 3", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 4", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 5", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 6", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 7", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 8", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));
    numbers.emplace_back(GameObject("Num 9", new Argent::Component::Renderer::SpriteRenderer("./Resources/Images/font.png")));

    for (auto& num : numbers)
    {
        GetOwner()->AddChild(num);
        num->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    }
    numbers.at(0)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));
}

void OneDigit::Update()
{
    if (timer > 0)
    {
        DrawNum(rand());

        timer -= Argent::Timer::GetDeltaTime();
    }
    else
    {
        DrawNum(number);
    }
}

void OneDigit::Assign(int num,float time)
{
    number = num;

    timer = time;
}

void OneDigit::DrawNum(int num)
{
    for (auto& num : numbers)
    {
        num->GetTransform()->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    }
    numbers.at(num)->GetTransform()->SetScale(DirectX::XMFLOAT3(1, 1, 1));
}

void Number::Initialize()
{
    for (int i = digits; i < digits; ++i)
    {
        std::string n{std::to_string(i) + " degit"};
        numbers.emplace_back(new GameObject(n.c_str(), new OneDigit()));
    }

    for (auto& n : numbers)
    {
        GetOwner()->AddChild(n);
    }
}

void Number::Update()
{
}

void Number::Assign(int num)
{
    *number = num;

    //���ꂼ��̌��̐���������Ă��Ċe�����ɑ��
    for (int i = digits; i < digits; ++i)
    {
        int j{ 1 };//�����łق��������̐���������Ă���
        for (int k = 1; k < i; ++k)
        {
            j *= 10;
        }

        int num{ *number };
        num /= j;
        num %= 10;

        numbers.at(i)->GetComponent<OneDigit>()->Assign(num,0.5f * i);
    }
}
