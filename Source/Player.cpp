#include "Player.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"

Player::Player() :BaseActor("player")
{
    movement = 10;

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

    camera = GameObject::FindGameObject("Camera"); // ��������
        movement = 10.5f;

        {
            auto c = camera->GetComponent<Camera>();
            c->SetMaxRotation(DirectX::XMFLOAT4(100, 0, 0, 0));
			c->SetMinRotation(DirectX::XMFLOAT4(-100, 0, 0, 0));
        }
}




void Player::Update()
{
    switch (state)
    {
    case 0:
        //camera = Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->GetGameObject("Camera");
        camera = GameObject::FindGameObject("Camera"); // ��������
        movement = 10.5f;

        {
            auto c = camera->GetComponent<Camera>();
            c->SetMaxRotation(DirectX::XMFLOAT4(100, 370, 0, 0));
			c->SetMinRotation(DirectX::XMFLOAT4(-100, -10, 0, 0));
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

        DirectX::XMFLOAT4 mouseMovement{ mouseVec.y * sensitivity,mouseVec.x * sensitivity,0,0 };

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

        // �J�������̂��(��]�ł���悤�ɂ���)
        if (setRotation.y > 360)setRotation.y -= 360;
        if (setRotation.y < 0)setRotation.y += 360;
        

        t->CalcForward();

        if(useCameraControl)
			t->SetRotation(setRotation);
        //t->SetRotation(cameraRot + mouseMovement);
#endif

        break;

    }
    start = end;
    end = GetTickCount();
	MoveCamera();
    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());

    ray->SetRayStartPosition(GetTransform()->GetPosition());
    ray->SetRayDirection(GetTransform()->CalcForward());
    ray->SetRayLength(movement * Argent::Timer::GetDeltaTime());

#ifdef _DEBUG
    if(Argent::Input::GetKeyDown(KeyCode::O))
    {
	    useCameraControl = !useCameraControl;
    }
#endif
}

void Player::DrawDebug()
{
    if(ImGui::TreeNode(GetName().c_str()))
    {
        ImGui::Checkbox("UseCameraControl", &useCameraControl);
        static int frameTime = 0;
        ++frameTime;
        double elapsedTime = (double)(end - start) / 1000;
        deltaTime += elapsedTime;
        static int frame = 0;
        if(deltaTime > 1.0f)
        {
            frame = frameTime;
	        deltaTime = 0;
            frameTime = 0;
        }
        ImGui::InputInt("FrameRate", &frame);
        ImGui::InputDouble("ElapsedTime", &elapsedTime);
        ImGui::SliderFloat("movement", &movement, 0.1f, 10.0);
        ImGui::DragFloat2("mouse", &mousePos.x);
        ImGui::SliderFloat("sensitivity", &sensitivity, 0.1f, 2.0f);
		BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}

// �J�����̈ړ�
void Player::MoveCamera()
{
#if 0
    Transform* t = camera->GetTransform();
    auto velocity = t->CalcForward();
    auto pos = t->GetPosition();
    auto rot = t->GetRotation();

    float _speed = movement;

    if (Argent::Input::GetKey(KeyCode::W) || Argent::Input::GetKey(KeyCode::S) ||
        Argent::Input::GetKey(KeyCode::A) || Argent::Input::GetKey(KeyCode::D))
    {
        if (Argent::Input::GetKey(KeyCode::W))
        {
            velocity.z = _speed;
        }
        if (Argent::Input::GetKey(KeyCode::S))
        {
            velocity.z = -_speed;
        }
        if (Argent::Input::GetKey(KeyCode::D))
        {
            velocity.x = _speed;
        }
        if (Argent::Input::GetKey(KeyCode::A))
        {
            velocity.x = -_speed;
        }
        if (velocity.x != 0 || velocity.z != 0)
        {
            pos.x += rot.x * velocity.x;
            pos.y = rot.y * velocity.y;
            pos.z = rot.z * velocity.z;
        };
        t->SetPosition(pos);
    }
#endif

    if(!useCameraControl) return;
    // �O(W)
	//if (GetAsyncKeyState('W') < 0)
    if (Argent::Input::GetKey(KeyCode::W))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
        start = GetTickCount();
    }
    

    // ���(S)
    //if (GetAsyncKeyState('S') < 0)
    if (Argent::Input::GetKey(KeyCode::S))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
     
    // �E(D)
    //if (GetAsyncKeyState('D') < 0)
    if (Argent::Input::GetKey(KeyCode::D))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // ��(A)
    //if (GetAsyncKeyState('A') < 0)
	if (Argent::Input::GetKey(KeyCode::A))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
}
