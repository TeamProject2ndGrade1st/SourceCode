#include "Numbers.h"

//数字フォントの処理

void OneDigit::Initialize()
{
    BaseActor::Initialize();

    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/0.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/1.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/2.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/3.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/4.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/5.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/6.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/7.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/8.png"));
    numbers.emplace_back(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Number/9.png"));

    for (auto& num : numbers)
    {
        GetOwner()->AddComponent(num);
        num->SetScale(DirectX::XMFLOAT3(0, 0, 0));
        num->SetOffset(DirectX::XMFLOAT3(offsetX * GetOwner()->GetTransform()->GetScale().x, 0, 0));
    }
    numbers.at(0)->SetScale(DirectX::XMFLOAT3(1, 1, 1));
}

void OneDigit::Update()
{
    if (timer > 0)
    {
        DrawNum(rand() % 10);

        timer -= Argent::Timer::GetDeltaTime();
    }
    else
    {
        DrawNum(number);
    }

    float o = offsetX * GetOwner()->GetTransform()->GetScale().x;
    for (auto& num : numbers)
    {
        num->SetOffset(DirectX::XMFLOAT3(o, 0, 0));
    }
}

void OneDigit::Assign(int num, float time)
{
    number = num;

    timer = time;
}

void OneDigit::DrawNum(int num)
{
    for (auto& num : numbers)
    {
        num->SetScale(DirectX::XMFLOAT3(0, 0, 0));
    }
    numbers.at(num)->SetScale(DirectX::XMFLOAT3(1, 1, 1));
}

void Number::Initialize()
{
    BaseActor::Initialize();

    for (int i = 0; i < digits; ++i)
    {
        std::string n{std::to_string(i) + " degit"};
        numbers.emplace_back(new OneDigit(i* -28.0f));
    }

    for (auto& n : numbers)
    {
        //GetOwner()->AddComponent(n);
        //auto* num = GameObject::Instantiate("number",n);
        auto* num = new GameObject("number",n);
        GetOwner()->AddChild(num);
    }
}

void Number::Update()
{
    auto pos = GetOwner()->GetTransform()->GetPosition();
    auto scale = GetOwner()->GetTransform()->GetScale();
    for (auto& num : numbers)
    {
        num->GetOwner()->GetTransform()->SetPosition(pos);
        num->GetOwner()->GetTransform()->SetScale(scale);
    }
}

void Number::DrawDebug()
{
    static int n;
    ImGui::DragInt("number", &n);
    if (ImGui::Button("SetNumber"))
    {
        Assign(n);
    }
}

void Number::Assign(int num)
{
    if (num < 0)num = 0;

    *number = num;

    //それぞれの桁の数字を取ってきて各数字に代入
    for (int i = 0; i < digits; ++i)
    {
        int j{ 1 };//こいつでほしい桁数の数字を取ってくる
        for (int k = 0; k < i; ++k)
        {
            j *= 10;
        }

        int num{ *number };
        num /= j;
        num %= 10;

        numbers.at(i)->Assign(num,0.3f + 0.3f * i);
    }
}

