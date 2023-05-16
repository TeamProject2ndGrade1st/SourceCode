#include "Player.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"
#include "Shadow.h"
#include "Grenade.h"

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

    //  camera = GameObject::FindByName("Camera"); // こっちで
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
    Turn();
    ThrowGrenade();

    if (camera == GameObject::FindByName("SecondCamera"))return;

    UpdateVerticalMove();
   
    MoveCamera();

    //ジャンプ
    if (Argent::Input::GetKeyDown(KeyCode::Space))
    {
        Jump(jumpPower);
    }

    FlyUpdate();

    // デバッグ用
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

    GetTransform()->SetPosition(camera->GetTransform()->GetPosition());
    GetTransform()->SetRotation(camera->GetTransform()->GetRotation());
}

void Player::DrawDebug()
{
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat("Gravity", &gravity, 0, 300);
        ImGui::SliderFloat("JumpPower", &jumpPower, 0, 300);
        ImGui::SliderFloat("FlyPower", &flyPower, 0, 1000);
        ImGui::SliderFloat("FlyEnergy", &flyEnergy, 0, 5);
        ImGui::SliderFloat("MaxFlyEnergy", &maxFlyEnergy, 0, 5);
        

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

void Player::Turn()
{
    // マウスの位置を取る
    mousePos = Argent::Input::Mouse::Instance().GetPosition();
    // マウスの移動量を取る
    DirectX::XMFLOAT2 mouseVec = Argent::Input::Mouse::Instance().GetMoveVec();


    // カメラのtransformを取る
    Transform* t = camera->GetTransform();
    // カメラの回転値を取るw
    DirectX::XMFLOAT4 cameraRot = t->GetRotation();

    DirectX::XMFLOAT4 mouseMovement{ mouseVec.y * sensitivity,mouseVec.x * sensitivity,0,0 };

    DirectX::XMFLOAT4 setRotation{};
    setRotation = cameraRot;
    setRotation.x += mouseMovement.x;
    setRotation.y += mouseMovement.y;

    // カメラ横のやつ(回転できるようにする)
    if (setRotation.y > 360)setRotation.y -= 360;
    if (setRotation.y < 0)setRotation.y += 360;

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
#endif
}

void Player::ThrowGrenade()
{
    if(Argent::Input::GetKeyDown(KeyCode::G))
    {
		GameObject::Instantiate("Grenade", new Grenade(Grenade::Mode::Machine, GetOwner()->GetTransform()->GetPosition(), GetOwner()->GetTransform()->CalcForward()));
    }
}

void Player::UpdateVerticalMove()
{
    auto pos = camera->GetTransform()->GetPosition();

    //着地
    if (pos.y < groundPosY && velocity.y < 0)
    {
        pos.y = groundPosY;
        velocity.y = 0;
        gun->AddRecoil(DirectX::XMFLOAT3(0,5, 0),0.2f);
        isGround = true;
    }

    if (!isGround)
    {
        float deltaTime = Argent::Timer::ArTimer::Instance().DeltaTime();
        velocity.y -= gravity * deltaTime;

        pos.y += velocity.y * deltaTime;

        if (pos.y > maxPosY)
        {
            pos.y = maxPosY;
            velocity.y /= 2;
        }

    }
    camera->GetTransform()->SetPosition(pos);
}

void Player::Jump(float power)
{
    if (!isGround)return;

    velocity.y += power;
    gun->AddRecoil(DirectX::XMFLOAT3(0,-10,0),0.08f);
    
    isGround = false;
}

void Player::Fly(float power)
{
    if (isGround)return;
    if (flyEnergy <= 0)return;

    velocity.y += power * Argent::Timer::ArTimer::Instance().DeltaTime();
    static float time;
    time += Argent::Timer::ArTimer::Instance().DeltaTime();
    flyEnergy -= Argent::Timer::ArTimer::Instance().DeltaTime();
    if (time > 0.05f)
    {
        gun->AddRecoil(DirectX::XMFLOAT3(0, -0.2, 0), 0.08f);
        time = 0;
    }
}

void Player::FlyUpdate()
{

    //ジャンプしてからすぐ飛ぶと
    static bool jump;
    static float timer;
    if (jump != isGround)
    {
        jump = isGround;
        if (isGround == false)
        {
            timer = 0.3f;
        }
    }
    
    timer -= Argent::Timer::ArTimer::Instance().DeltaTime();

    

    //上昇
    if (Argent::Input::GetKey(KeyCode::Space))
    {
        if (timer <= 0)
        {
            Fly(flyPower);
        }
    }
    else
    {
        if (maxFlyEnergy <= flyEnergy)
        {
            flyEnergy = maxFlyEnergy;
        }
        else
        {
            flyEnergy += Argent::Timer::ArTimer::Instance().DeltaTime() * 0.8f;
        }
    }


}

// カメラの移動
void Player::MoveCamera()
{
    // 入力情報を取得
    float ax = 0;
    float ay = 0;

    if (Argent::Input::GetKey(KeyCode::D))ax = 1.0f;
    if (Argent::Input::GetKey(KeyCode::A))ax = -1.0f;
    if (Argent::Input::GetKey(KeyCode::W))ay = 1.0f;
    if (Argent::Input::GetKey(KeyCode::S))ay = -1.0f;

    if (!(ax == 0 && ay == 0))gun->walking = true;
    else gun->walking = false;

    // カメラ方向とスティック入力値によって進行方向を計算する
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

        //進行方向に対して垂直方向に少しずらす
        DirectX::XMFLOAT3 offset;
        DirectX::XMStoreFloat3(&offset, DirectX::XMVectorScale(RIGHT, 1.f));

        //少し後ろに下げる
        pos = pos - direction * 3.0f;
        ray->SetRayData(pos + offset, p);
        if (Argent::Collision::RayCollisionDetection(ray, hitResult, GameObject::Tag::Stage))
        {
            hitResult.position.y = pos.y;
            p = hitResult.position;
        }
        else
        {
            //さっきとは判定方向にずらして判定を取る
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
