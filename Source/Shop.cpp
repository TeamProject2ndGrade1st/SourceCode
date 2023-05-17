#include "Shop.h"


void Shop::Initialize()
{
    BaseComponent::Initialize();

    GetOwner()->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/UI_0516_2.png"));

    GetOwner()->ReplaceTag(GameObject::Tag::Shop);

    GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(Argent::Graphics::GetWindowWidth(), 0, 0));

    easeFunc = &Easing::OutExp;

#ifdef _DEBUG
    currentEase = "OutExp";
    std::string s = "OutExp"; easeFunctoins.push_back(s);
    s = "OutSine";easeFunctoins.push_back(s);
    s = "OutQuad";easeFunctoins.push_back(s);
    s = "OutCubic";easeFunctoins.push_back(s);
    s = "OutQuart";easeFunctoins.push_back(s);
    s = "OutBounce";easeFunctoins.push_back(s);

    easeFuncs.push_back(&Easing::OutExp);
    easeFuncs.push_back(&Easing::OutSine);
    easeFuncs.push_back(&Easing::OutQuad);
    easeFuncs.push_back(&Easing::OutCubic);
    easeFuncs.push_back(&Easing::OutQuart);
    easeFuncs.push_back(&Easing::OutBounce);
#endif // _DEBUG                

}

void Shop::Begin()
{
    static bool callOnce = [&]() {
        std::vector<GameObject*> m;
        GameObject::FindByTag(GameObject::Tag::ChangeMode, m);
        mode = m.at(0)->GetComponent<ChangeMode>();
        return true;
    }();
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
        }
    }
    

    if (mode->openShop)OpenUpdate();
    else CloseUpdate();
}

void Shop::OpenUpdate()
{
    if (GetOwner()->GetTransform()->GetPosition().x >= 0)
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(
            (*easeFunc)(timer, easeMaxTime, 0.0f, Argent::Graphics::GetWindowWidth()),
            0,0)
        );
        if(timer < easeMaxTime)timer += Argent::Timer::GetDeltaTime();
        else easeEnd = true;
    }
}

void Shop::CloseUpdate()
{
    if (GetOwner()->GetTransform()->GetPosition().x <= Argent::Graphics::GetWindowWidth())
    {
        GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(
            (*easeFunc)(timer, easeMaxTime, Argent::Graphics::GetWindowWidth(), 0.0f),
            0, 0)
        );
        if (timer < easeMaxTime)timer += Argent::Timer::GetDeltaTime();
        else easeEnd = true;
    }
}

void Shop::DrawDebug()
{
    ImGui::SliderFloat("Timer", &timer,0,easeMaxTime);
    ImGui::DragFloat("MaxTimer", &easeMaxTime,0.01f);

    ImGuiComboUI("Easing",currentEase,easeFunctoins);
}

void Item::Update(float x, float y)
{
    float left = pos.x - button.left;
    float right = pos.x + button.right;
    float top = pos.y + button.top;
    float bottom = pos.y - button.bottom;

    if (left > pos.x)   return;
    if (right < pos.x)  return;
    if (top < pos.y)  return;
    if (bottom > pos.y)   return;


}
