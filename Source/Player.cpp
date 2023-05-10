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
    ray = new Argent::Component::Collision::RayCast();
    auto g = GetOwner();
    g->AddComponent(ray);



    movement = 50.5f;
    //  camera = GameObject::FindByName("Camera"); // ��������
    //  camera->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 16.0f, 0));
    //  {
    //      auto c = camera->GetComponent<Camera>();
          //c->SetMaxRotation(DirectX::XMFLOAT4(70, 370, 0, 0));
          //c->SetMinRotation(DirectX::XMFLOAT4(-70, -10, 0, 0));
    //  }


    gun = new BaseGun("Base Gun");
    //gun = GameObject::Instantiate("Gun", new BaseGun("Base Gun"));
    GetOwner()->AddComponent(gun);

    std::vector<GameObject*> gameObject;
    GameObject::FindByTag(GameObject::Tag::MainCamera, gameObject);
    for (auto g : gameObject)
    {
        cameraArray.emplace_back(g->GetComponent<Camera>());
    }

    for (auto& c : cameraArray)
    {
        if (c->GetIsSceneCamera())
        {
            camera = c->GetOwner();
            camera->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 16.0f, 0));
            c->SetMaxRotation(DirectX::XMFLOAT4(70, 370, 0, 0));
            c->SetMinRotation(DirectX::XMFLOAT4(-70, -10, 0, 0));

            break;
        }
    }

}

void Player::Begin()
{
    for (auto& c : cameraArray)
    {
        if (c->GetIsSceneCamera())
        {
            camera = c->GetOwner();
            break;
        }
    }
}

void Player::Update()
{
    // switch (state)
    // {
    // case 0:
    //     //camera = Argent::Scene::SceneManager::Instance()->GetCurrentScene()->GetGameObject("Camera");
    ////     camera = GameObject::FindByName("Camera"); // ��������
    ////     movement = 50.5f;

    ////     {
    ////         auto c = camera->GetComponent<Camera>();
    ////         c->SetMaxRotation(DirectX::XMFLOAT4(70, 370, 0, 0));
             ////c->SetMinRotation(DirectX::XMFLOAT4(-70, -10, 0, 0));
    ////     }

    //     ++state;
    //     break;
    // case 1:

         // �ړ�
    MoveCamera();

    // �f�o�b�O�p
#ifdef _DEBUG
    if (Argent::Input::GetKey(KeyCode::Space))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.y += 0.1f;
        t->SetPosition(pos);
    }
    if (Argent::Input::GetKey(KeyCode::M))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.y -= 0.1f;
        t->SetPosition(pos);
    }
#endif

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




    // �J�������̂��(��]�ł���悤�ɂ���)
    if (setRotation.y > 360)setRotation.y -= 360;
    if (setRotation.y < 0)setRotation.y += 360;
#endif
#ifdef _DEBUG
    static bool use = false;

    if (Argent::Input::GetKeyUp(KeyCode::U))
    {
        use = !use;
    }
    if (use)
    {
        t->SetRotation(setRotation);
    }
#else
    t->SetRotation(setRotation);
#endif
    //   break;
  // }

    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());
    GetTransform()->SetRotation(camera->GetTransform()->GetRotation());


    //�e�̈ʒu

    DirectX::XMFLOAT3 forward = GetTransform()->CalcForward();
    DirectX::XMFLOAT3 up = GetTransform()->CalcUp();
    DirectX::XMFLOAT3 right = GetTransform()->CalcRight();

    DirectX::XMFLOAT3 NormForward = forward;
    NormForward.y = 0;

    float dot{};
    DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&forward)), DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&NormForward))));

    //DirectX::XMFLOAT3 offsetPosition = forward * gunOffset.z + up * gunOffset.y + right * gunOffset.x;
    //gun->GetTransform()->SetPosition(offsetPosition + GetOwner()->GetTransform()->GetPosition());

    //DirectX::XMFLOAT4 setRotation = GetOwner()->GetTransform()->GetRotation();
    //setRotation = Absolute(setRotation);
    //setRotation.x = DirectX::XMConvertToDegrees(acosf(dot));
    //gun->GetTransform()->SetRotation(setRotation);

}

void Player::DrawDebug()
{
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::Checkbox("UseCameraControl", &useCameraControl);
        ImGui::SliderFloat("movement", &movement, 0.1f, 10.0);
        ImGui::DragFloat2("mouse", &mousePos.x);
        ImGui::SliderFloat("sensitivity", &sensitivity, 0.1f, 2.0f);
        ImGui::SliderFloat("OffsetLength", &offsetLength, 0.0f, 10.0f);
        ImGui::DragFloat3("GunOffset", &gunOffset.x, 0.1f, -FLT_MAX, FLT_MAX);
        BaseActor::DrawDebug();
        ImGui::TreePop();
    }
}

// �J�����̈ړ�
void Player::MoveCamera()
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
    auto p = t->GetPosition();


    DirectX::XMFLOAT3 cameraRight = t->CalcRight();
    DirectX::XMFLOAT3 cameraFront = t->CalcForward();


    cameraRight.y = 0;
    cameraFront.y = 0;

    cameraRight = Normalize(cameraRight);
    cameraFront = Normalize(cameraFront);


    cameraRight = cameraRight * ax;
    cameraFront = cameraFront * ay;
    cameraRight = cameraRight * movement * Argent::Timer::GetDeltaTime();
    cameraFront = cameraFront * movement * Argent::Timer::GetDeltaTime();


    DirectX::XMFLOAT3 direction = Normalize(cameraRight + cameraFront);

    p = p + cameraRight + cameraFront;

    auto pos = t->GetPosition();


    ray->SetRayData(pos, p);
    HitResult hitResult{};
    if (Argent::Collision::RayCollisionDetection(ray, hitResult/*, GameObject::Tag::Stage*/))
    {
        // hitResult.position.y = GetTransform()->GetPosition().y;
        hitResult.position.y = pos.y;
        p = hitResult.position;
    }
    else
    {
        const DirectX::XMVECTOR UP = DirectX::XMVectorSet(0, 1, 0, 0);
        DirectX::XMVECTOR RIGHT = DirectX::XMVector3Cross(UP, DirectX::XMLoadFloat3(&direction));

        //�i�s�����ɑ΂��Đ��������ɏ������炷
        DirectX::XMFLOAT3 offset;
        DirectX::XMStoreFloat3(&offset, DirectX::XMVectorScale(RIGHT, 1.f));

        //�������ɉ�����
        pos = pos - direction * 3.0f;
        ray->SetRayData(pos + offset, p);
        if (Argent::Collision::RayCollisionDetection(ray, hitResult, GameObject::Tag::Stage))
        {
            hitResult.position.y = pos.y;
            p = hitResult.position;
        }
        else
        {
            //�������Ƃ͔�������ɂ��炵�Ĕ�������
            offset = offset * -1.0f;
            ray->SetRayData(pos + offset, p);
            if (Argent::Collision::RayCollisionDetection(ray, hitResult, GameObject::Tag::Stage))
            {
                hitResult.position.y = pos.y;
                p = hitResult.position;
            }
        }
    }

    t->SetPosition(p);
}
