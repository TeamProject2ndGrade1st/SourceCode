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

    camera = GameObject::FindGameObject("Camera"); // こっちで
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
        camera = GameObject::FindGameObject("Camera"); // こっちで
        movement = 10.5f;

        {
            auto c = camera->GetComponent<Camera>();
            c->SetMaxRotation(DirectX::XMFLOAT4(100, 370, 0, 0));
			c->SetMinRotation(DirectX::XMFLOAT4(-100, -10, 0, 0));
        }

        ++state;
        break;
    case 1:

        // 移動
        //MoveCamera();
        InputMove(Argent::Timer::GetDeltaTime());

        // マウスのポジション
#if 1
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


        // 制限を作る
        //if (setRotation.x >= 100.0f)
        //{
        //    cameraRot.x = 100;
        //	mouseMovement.y = 0;
        //    mouseMovement.x = 0;
        //   // mouseMovement.x = -0.01;
        //    setRotation.x = 100;
        //}

        // カメラ横のやつ(回転できるようにする)
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

// カメラの移動
void Player::MoveCamera()
{

    static DWORD start;
    static DWORD end;


    // 前(W)
	//if (GetAsyncKeyState('W') < 0)
    if (Argent::Input::GetKey(KeyCode::W))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
        start = GetTickCount();
    }
    

    // 後ろ(S)
    //if (GetAsyncKeyState('S') < 0)
    if (Argent::Input::GetKey(KeyCode::S))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.z -= movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }
     
    // 右(D)
    //if (GetAsyncKeyState('D') < 0)
    if (Argent::Input::GetKey(KeyCode::D))
    {
        auto t = camera->GetTransform();
        auto pos = t->GetPosition();
        pos.x += movement * Argent::Timer::ArTimer::Instance().DeltaTime();
        t->SetPosition(pos);
    }

    // 左(A)
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
    // 入力情報を取得
    float ax = 0;
    float ay = 0;

    if (Argent::Input::GetKey(KeyCode::D))ax = 1.0f;
    if (Argent::Input::GetKey(KeyCode::A))ax = -1.0f;
    if (Argent::Input::GetKey(KeyCode::W))ay = 1.0f;
    if (Argent::Input::GetKey(KeyCode::S))ay = -1.0f;

    // カメラ方向とスティック入力値によって進行方向を計算する
    Transform* t = camera->GetTransform();

    const DirectX::XMFLOAT3& cameraRight = t->CalcRight();
    const DirectX::XMFLOAT3& cameraFront = t->CalcForward();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする

    // カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = (cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化
        DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(cameraRightX, 0.0f, cameraRightZ);

        // 一個ずつ計算した場合
        DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);  // ベクトルを作る
        DirectX::XMVector3Normalize(vec);                   // 正規化する
        DirectX::XMStoreFloat3(&v, vec);                    // FLOAT3に戻す

        // まとめていっきにした場合
        //DirectX::XMStoreFloat3(&v, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v)));

        cameraRightX = v.x;
        cameraRightZ = v.z;
    }

    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = (cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(cameraFrontX, 0.0f, cameraFrontZ);
        DirectX::XMStoreFloat3(&v, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v)));

        cameraFrontX = v.x;
        cameraFrontZ = v.z;
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行方向ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = cameraRightX * sinf(ax) + cameraFrontX * sinf(ay);
    vec.z = cameraRightZ * sinf(ax) + cameraFrontZ * sinf(ay);
    // Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

void Player::InputMove(float elapsedTime)
{
    // 進行方向ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    // 移動処理
    Move(elapsedTime, moveVec.x, moveVec.z, 5.0f);

    // 旋回処理
    //Turn(elapsedTime, moveVec.x, moveVec.z, DirectX::XMConvertToRadians(720));
}

// 移動処理
void Player::Move(float elapsedTime, float vx, float vz, float speed)
{
    Transform* t = camera->GetTransform();
    DirectX::XMFLOAT3 pos = t->GetPosition();

    speed *= elapsedTime;
    pos.z += vz * speed;
    pos.x += vx * speed;

    t->SetPosition(pos);
}

// 旋回処理
void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
    Transform* t = camera->GetTransform();
    DirectX::XMFLOAT4 angle = t->GetRotation();

    speed *= elapsedTime;

    // 進行ベクトルがゼロベクトル場合は処理する必要なし
    if (vx == 0 && vz == 0)return;

    // 進行ベクトルを単位ベクトル化
    DirectX::XMFLOAT3 v = DirectX::XMFLOAT3(vx, 0.0f, vz);  // FLOAT3作る
    DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&v);      // VECTOR作る
    DirectX::XMVector3Normalize(vec);                       // 正規化する
    DirectX::XMStoreFloat3(&v, vec);

    // 自身の回転値から前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    // 回転角を求めるため、２つの単位ベクトルの内積を計算する
    float dot = (v.x * frontX) + (v.z * frontZ);

    // 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
    // 小さいほど1.0に近づくという性質を利用して回転速度を調整する
    float rot = 1.0 - dot;  // 補正値

    // 左右判定を行うために２つの単位ベクトルの外積を計算する
    float cross = (v.x * frontZ) - (v.z * frontX);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
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