#include "Player.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"

Player::Player() :BaseActor("player")
{
    movement = 10;
    sensitivity = 0.3f;
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


void Player::DrawDebug()
{
    if(ImGui::TreeNode(GetName().c_str()))
    {
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
        //MoveCamera();
        InputMove(Argent::Timer::GetDeltaTime());

        // �}�E�X�̃|�W�V����
#if 1
        // �}�E�X�̈ʒu�����
        mousePos = Argent::Input::Mouse::Instance().GetPosition();
        // �}�E�X�̈ړ��ʂ����
        DirectX::XMFLOAT2 mouseVec = Argent::Input::Mouse::Instance().GetMoveVec();
        
        
        // �J������transform�����
        Transform* t = camera->GetTransform();
        // �J�����̉�]�l�����w
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

        t->SetRotation(setRotation);
        

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
}

// �J�����̈ړ�
void Player::MoveCamera()
{

    static DWORD start;
    static DWORD end;


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

#if 1
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
    // ���͏����擾
    float ax = 0;
    float ay = 0;

    if (Argent::Input::GetKey(KeyCode::D))ax = 1.0f;
    if (Argent::Input::GetKey(KeyCode::A))ax = -1.0f;
    if (Argent::Input::GetKey(KeyCode::W))ay = 1.0f;
    if (Argent::Input::GetKey(KeyCode::S))ay = -1.0f;

    // �J���������ƃX�e�B�b�N���͒l�ɂ���Đi�s�������v�Z����
    Transform* t = camera->GetTransform();

    const DirectX::XMFLOAT3& cameraRight = t->CalcRight();
    const DirectX::XMFLOAT3& cameraFront = t->CalcForward();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    // �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = (cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����
        DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(cameraRightX, 0.0f, cameraRightZ);

        // ����v�Z�����ꍇ
        DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);  // �x�N�g�������
        DirectX::XMVector3Normalize(vec);                   // ���K������
        DirectX::XMStoreFloat3(&v, vec);                    // FLOAT3�ɖ߂�

        // �܂Ƃ߂Ă������ɂ����ꍇ
        //DirectX::XMStoreFloat3(&v, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v)));

        cameraRightX = v.x;
        cameraRightZ = v.z;
    }

    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = (cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����
        DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(cameraFrontX, 0.0f, cameraFrontZ);
        DirectX::XMStoreFloat3(&v, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v)));

        cameraFrontX = v.x;
        cameraFrontZ = v.z;
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�����x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = cameraRightX * sinf(ax) + cameraFrontX * sinf(ay);
    vec.z = cameraRightZ * sinf(ax) + cameraFrontZ * sinf(ay);
    // Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

void Player::InputMove(float elapsedTime)
{
    // �i�s�����x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // �ړ�����
    Move(elapsedTime, moveVec.x, moveVec.z, 5.0f);

    // ���񏈗�
    //Turn(elapsedTime, moveVec.x, moveVec.z, DirectX::XMConvertToRadians(720));
}

// �ړ�����
void Player::Move(float elapsedTime, float vx, float vz, float speed)
{
    Transform* t = camera->GetTransform();
    DirectX::XMFLOAT3 pos = t->GetPosition();

    speed *= elapsedTime;
    pos.z += vz * speed;
    pos.x += vx * speed;

    t->SetPosition(pos);
}

// ���񏈗�
void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
    Transform* t = camera->GetTransform();
    DirectX::XMFLOAT4 angle = t->GetRotation();

    speed *= elapsedTime;

    // �i�s�x�N�g�����[���x�N�g���ꍇ�͏�������K�v�Ȃ�
    if (vx == 0 && vz == 0)return;

    // �i�s�x�N�g����P�ʃx�N�g����
    DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(vx, 0.0f, vz);  // FLOAT3���
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);      // VECTOR���
    DirectX::XMVector3Normalize(vec);                       // ���K������
    DirectX::XMStoreFloat3(&v, vec);

    // ���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    // ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
    float dot = (v.x * frontX) + (v.z * frontZ);

    // ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
    // �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    float rot = 1.0 - dot;  // �␳�l

    // ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (v.x * frontZ) - (v.z * frontX);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        //angle.y -= speed;
        angle.y -= rot * speed;
    }
    else
    {
        //angle.y += speed;
        angle.y += rot * speed;
    }

    t->SetRotation(angle);
}
#endif