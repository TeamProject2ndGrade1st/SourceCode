#include "Player.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"

Player::Player() :BaseActor("player")
{
}

void Player::Initialize()
{
    static bool b = false;
    if(!b)
    {
	    ray = new Argent::Component::Collision::RayCast();
	    auto g = GetOwner();
	    g->AddComponent(ray);
        b = true;
    }
}

#ifdef _DEBUG
void Player::DrawDebug()
{
    if(ImGui::TreeNode(GetName().c_str()))
    {
        ImGui::SliderFloat("movement", &movement, 0.1f, 10.0);
		BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}

void Player::OnRayCollision(const Argent::Component::Collider::RayCastCollider* other)
{
	//GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 0, -10));
	camera->GetTransform()->SetPosition(GetTransform()->GetPosition());
}
#endif

void Player::Update()
{
    switch (state)
    {
    case 0:
        //camera = Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->GetGameObject("Camera");
        camera = GameObject::FindGameObject("Camera"); // ‚±‚Á‚¿‚Å
        movement = 0.5f;

        ++state;
        break;
    case 1:

        // ˆÚ“®
        MoveCamera();

        break;

    }

    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());
    auto p = GetTransform()->GetPosition();
    auto f = GetTransform()->CalcForward();
    auto l = movement* Argent::Timer::GetDeltaTime();
    ray->SetRayStartPosition(p);
    ray->SetRayDirection(f);
    ray->SetRayLength(l);
}

// ƒJƒƒ‰‚ÌˆÚ“®
void Player::MoveCamera()
{
    // ‘O(W)
    if (Argent::Input::GetKey(KeyCode::W))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // Œã‚ë(S)
    if (Argent::Input::GetKey(KeyCode::S))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // ‰E(D)
    if (Argent::Input::GetKey(KeyCode::D))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // ¶(A)
    if (Argent::Input::GetKey(KeyCode::A))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
}
