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
    _ASSERT_EXPR(false, L"Succeeded");
}
#endif

void Player::Update()
{
    switch (state)
    {
    case 0:
        //camera = Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->GetGameObject("Camera");
        camera = GameObject::FindGameObject("Camera"); // こっちで
        movement = 0.5f;

        ++state;
        break;
    case 1:

        // 移動
        MoveCamera();

        break;

    }
    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());

    ray->SetRayStartPosition(GetTransform()->GetPosition());
    ray->SetRayDirection(GetTransform()->CalcForward());
    ray->SetRayLength(movement * Argent::Timer::GetDeltaTime());
}

// カメラの移動
void Player::MoveCamera()
{
    // 前(W)
    if (Argent::Input::GetKey(KeyCode::W))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // 後ろ(S)
    if (Argent::Input::GetKey(KeyCode::S))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // 右(D)
    if (Argent::Input::GetKey(KeyCode::D))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // 左(A)
    if (Argent::Input::GetKey(KeyCode::A))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
}
