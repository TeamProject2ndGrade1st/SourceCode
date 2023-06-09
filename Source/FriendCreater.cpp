#include "FriendCreater.h"
#include <d3d12.h>
#include <vector>
#include "FriendCreature.h"
#include "FriendDrone.h"

void FriendCreater::Initialize()
{
    BaseActor::Initialize();
    //GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));

    imageFriend[0] = new GameObject("ImageCreature", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver9.fbx", false));
    imageFriend[1] = new GameObject("ImageDrone", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0516_2.fbx", false));
    imageFriend[1]->GetTransform()->SetScaleFactor(1.8f);
   
    GetOwner()->AddChild(imageFriend[0]);
    GetOwner()->AddChild(imageFriend[1]);

    GetOwner()->GetTransform()->SetScaleFactor(0.15f);
    GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>();
    friendManager = GameObject::FindByName("FriendManager")->GetComponent<FriendManager>();

    GetOwner()->ReplaceTag(GameObject::Tag::FriendCreatar);

    //専用のカメラ生成
    GameObject::Instantiate("SecondCamera", camera = new Camera(false, 1280, 720));
    camera->GetOwner()->GetTransform()->SetPosition(DirectX::XMFLOAT3(0,200,0));
    camera->GetOwner()->GetTransform()->SetRotation(DirectX::XMFLOAT4(70,180,0,0));
}

void FriendCreater::Update()
{
    ImagineFriendUpdate();

    std::vector<GameObject*> f;
    GetOwner()->GetChildArray(f);
    for (auto& fr : f )
    {
        //ふっとばす
        fr->GetTransform()->AddPosition(DirectX::XMFLOAT3(1000000, 0, 0));
    }
    //片方もどす
    float fPosY = f.at(createType)->GetTransform()->GetPosition().y;
    f.at(createType)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0,fPosY,0));

    //色変更
    //スキンドメッシュレンダラーとメッシュレンダラー用の処理を作る
    if (f.at(0)->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetMaterial()->color.color.x != color.x)
    {
        f.at(0)->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetMaterial()->color.color = color;
    }
    if (f.at(1)->GetComponent<Argent::Component::Renderer::MeshRenderer>()->GetMaterial()->color.color.x != color.x)
    {
        f.at(1)->GetComponent<Argent::Component::Renderer::MeshRenderer>()->GetMaterial()->color.color = color;
    }
    

    if (Argent::Input::GetKeyUp(KeyCode::D1))
    {
        createType = static_cast<int>(FriendManager::Type::Creature);
    }
    if (Argent::Input::GetKeyDown(KeyCode::D2))
    {
        createType = static_cast<int>(FriendManager::Type::Drone);
    }

    //生成範囲外にでたらドラッグと生成の処理をしない
    if (!canCreate)
    {
        //もし範囲外でドラッグ中の味方がいる状態でクリックを離したらその味方を消す
        if (!dragTarget)return;
        if (Argent::Input::Mouse::Instance().GetButtonUp(Argent::Input::Mouse::Button::LeftButton))
        {
            dragTarget->Destroy(dragTarget);

            if (dragTarget->GetComponent<FriendCreature>())
            {
                canCreateNumber[static_cast<int>(FriendManager::Type::Creature)]++;
            }
            if (dragTarget->GetComponent<FriendDrone>())
            {
                canCreateNumber[static_cast<int>(FriendManager::Type::Drone)]++;
            }
        }

        return;
    }

    if (!DragFriend())
    {
        SetFriendByClick();
    }
}

void FriendCreater::DrawDebug()
{
    BaseActor::DrawDebug();
    if (ImGui::TreeNode("FriendCreater"))
    {
        ImGui::SliderFloat3("CanCreatePos", &createPos.x, 0, 1000.0f);
        ImGui::TreePop();
    }
}

void FriendCreater::SetFriendByClick()
{
    //エネミー配置処理
    if (Argent::Input::Mouse::Instance().GetButtonDown(Argent::Input::Mouse::Button::RightButton))
    {
        if (canCreateNumber[createType] <= 0)return;
        DirectX::XMFLOAT3 pos = { GetTransform()->GetPosition() };
        pos.y = 0;
        switch (createType)
        {
        case static_cast<int>(FriendManager::Type::Creature):
            friendManager->AddFriend(new FriendCreature(pos));
            break;
        case static_cast<int>(FriendManager::Type::Drone):
            friendManager->AddFriend(new FriendDrone(pos));
            break;
        }

        //生成可能数の減少
        canCreateNumber[createType]--;
    }
}

void FriendCreater::ImagineFriendUpdate()
{
    //ビューポート
    D3D12_VIEWPORT viewport = Argent::Graphics::Graphics::Instance()->GetViewport();

    //交換行列
    DirectX::XMMATRIX View = camera->GetViewMatrix();
    DirectX::XMMATRIX Projection = camera->GetProjectionMatrix();
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    DirectX::XMVECTOR startPosition = { Argent::Graphics::GetWindowWidth() / 2,Argent::Graphics::GetWindowHeight() / 2,0.0f };
    DirectX::XMVECTOR endPosition = { Argent::Graphics::GetWindowWidth() / 2,Argent::Graphics::GetWindowHeight() / 2,1.0f };

    DirectX::XMVECTOR Start = DirectX::XMVector3Unproject(
        startPosition,
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        viewport.MinDepth, viewport.MaxDepth,
        Projection, View, World
    );
    DirectX::XMVECTOR End = DirectX::XMVector3Unproject(
        endPosition,
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        viewport.MinDepth, viewport.MaxDepth,
        Projection, View, World
    );

    DirectX::XMFLOAT3 start;
    DirectX::XMStoreFloat3(&start, Start);
    DirectX::XMFLOAT3 end;
    DirectX::XMStoreFloat3(&end, End);

    std::vector<GameObject*> stage;
    GameObject::FindByTag(GameObject::Tag::Stage, stage);
    auto meshResource = stage.at(0)->GetComponent<Argent::Component::Renderer::MeshRenderer>()->GetMesh()->meshResource;

    HitResult result;
    if (Argent::Helper::Collision::IntersectRayVsModel(start, end,
        meshResource, stage.at(0)->GetComponent<Argent::Component::Collider::RayCastCollider>()->GetWorldTransform(),
        result))
    {
        DirectX::XMFLOAT3 pos = { result.position.x,0,result.position.z };
        CanCreate(pos);
        if (canCreate)
        {
            if (createType == static_cast<int>(FriendManager::Type::Drone))
            {
                pos.y = 30;
            }
            GetTransform()->SetPosition(pos);
            
            color = { 0,0.3f,0.8f,0.5f };
        }
        else
        {
            color = { 1,0.1f,0.1f,0.5f };
        }
    }
}

bool FriendCreater::DragFriend()
{
    if (Argent::Input::Mouse::Instance().GetButton(Argent::Input::Mouse::Button::LeftButton))
    {
        if (!dragTarget)
        {
            //ドラッグ先がいなければ
            std::vector<GameObject*> f;
            GameObject::FindByTag(GameObject::Tag::Friend, f);
           for (auto it = f.begin(); it != f.end(); ++it)
            {
                float radius = (*it)->GetComponent<Argent::Component::Collider::SphereCollider>()->GetRadius();
                DirectX::XMVECTOR mPos = { GetTransform()->GetPosition().x,GetTransform()->GetPosition().z };
                DirectX::XMVECTOR fPos = { (*it)->GetTransform()->GetPosition().x,(*it)->GetTransform()->GetPosition().z };
                DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(mPos, fPos);
                float length = DirectX::XMVectorGetX(DirectX::XMVector2Length(vec));

                if (length < radius)
                {
                    dragTarget = (*it);
                    return true;
                }
            }
        }
        else
        {
            //すでにドラッグしている物があるなら
            dragTarget->GetTransform()->SetPosition(GetTransform()->GetPosition());
            return true;
        }
    }
    else dragTarget = nullptr;
    return false;
}

void FriendCreater::CanCreate(DirectX::XMFLOAT3 pos)
{
    canCreate = false;

    //生成可能数が０以下
    /*if (canCreateNumber[createType] <= 0)return;*/

    float left = createRange.left + createPos.x;
    float right = createRange.right + createPos.x;
    float front = createRange.top + createPos.z;
    float back = createRange.bottom + createPos.z;

    if (left > pos.x)   return;
    if (right < pos.x)  return;
    if (front < pos.z)  return;
    if (back > pos.z)   return;

    canCreate = true;
}
