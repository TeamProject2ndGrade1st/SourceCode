#include "Player.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"

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


void Player::DrawDebug()
{
    if(ImGui::TreeNode(GetName().c_str()))
    {
        ImGui::SliderFloat("movement", &movement, 0.1f, 10.0);
        ImGui::DragFloat2("mouse", &mousePos.x);
		BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}


void Player::Update()
{
    switch (state)
    {
    case 0:
        //camera = Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->GetGameObject("Camera");
        camera = GameObject::FindGameObject("Camera"); // ��������
        movement = 0.5f;

        {
            auto c = camera->GetComponent<Camera>();
            c->SetMaxRotation(DirectX::XMFLOAT4(100, 0, 0, 0));
        }

        ++state;
        break;
    case 1:

        // �ړ�
        MoveCamera();


        // �}�E�X�̃|�W�V����
#if 1
        // �}�E�X�̈ʒu�����
        mousePos = Argent::Input::Mouse::Instance().GetPosition();
        // �}�E�X�̈ړ��ʂ����
        DirectX::XMFLOAT2 mouseVec = Argent::Input::Mouse::Instance().GetMoveVec();
        
        
        
        // �J������transform�����
        Transform* t = camera->GetTransform();
        // �J�����̉�]�l�����
        DirectX::XMFLOAT4 cameraRot = t->GetRotation();

        DirectX::XMFLOAT4 mouseMovement{ mouseVec.y,mouseVec.x,0,0 };

        DirectX::XMFLOAT4 setRotation{};
        setRotation = cameraRot;
        setRotation.x += mouseMovement.x;
        setRotation.y += mouseMovement.y;
        // ���������
        //if (setRotation.x >= 100.0f)
        //{
        //    cameraRot.x = 100;
        //	mouseMovement.y = 0;
        //    mouseMovement.x = 0;
        //   // mouseMovement.x = -0.01;
        //    setRotation.x = 100;

        //}


        t->SetRotation(setRotation);
        //t->SetRotation(cameraRot + mouseMovement);
#endif

        break;

    }
    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());

    ray->SetRayStartPosition(GetTransform()->GetPosition());
    ray->SetRayDirection(GetTransform()->CalcForward());
    ray->SetRayLength(movement * Argent::Timer::GetDeltaTime());
}

// �J�����̈ړ�
void Player::MoveCamera()
{
    // �O(W)
    if (Argent::Input::GetKey(KeyCode::W))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // ���(S)
    if (Argent::Input::GetKey(KeyCode::S))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // �E(D)
    if (Argent::Input::GetKey(KeyCode::D))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // ��(A)
    if (Argent::Input::GetKey(KeyCode::A))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
}
