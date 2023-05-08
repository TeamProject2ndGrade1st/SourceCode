#include "FriendManager.h"
#include "FriendCreature.h"
#include "FriendDrone.h"
#include "Argent/Graphic/Graphics.h"

void FriendManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::FriendManager);
}

void FriendManager::Update()
{
    static DirectX::XMFLOAT3 pos{};
    pos.z = -440;
    if (Argent::Input::GetKeyUp(KeyCode::E))
    {
        AddFriend(new FriendCreature(pos));
             
    }

    for (auto activer = friendArray.begin();activer != friendArray.end();++activer)
    {
        for (auto passiver = activer + 1; passiver != friendArray.end(); ++passiver)
        {
            (*activer)->OnCollision((*passiver)->GetOwner()->GetComponent<Argent::Component::Collider::SphereCollider>());
        }
    }
    
    SetFriendByClick();
}

void FriendManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    static int type{};
    static int route{};
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -1000.0f, 1000.0f);
        ImGui::SliderInt("FriendType", &type, 0, static_cast<int>(Type::End)-1);
        ImGui::Text("1: Creature\n2: Drone\n");
        ImGui::SliderInt("Route", &route, 0, 2);
        ImGui::Text("Left:0,Center:1,Right:2");
        if (ImGui::Button("AddFriend"))
        {
            switch (type)
            {
            case static_cast<int>(Type::Creature)://クリーチャー
                AddFriend(new FriendCreature(pos,route));
                break;
            case static_cast<int>(Type::Drone):
                AddFriend(new FriendDrone(pos,route));
                break;
            }
        }
        ImGui::TreePop();
    }
    BaseActor::DrawDebug();
}

void FriendManager::AddFriend(BaseFriend* _friend)
{
    GameObject::Instantiate("Friend", _friend);
    /*scene->AddObject(new GameObject(
        "Friend", 
        _friend));*/
    friendArray.emplace_back(_friend);

    //タグ登録はそれぞれのフレンド本体で行っている
}

void FriendManager::SetFriendByClick()
{
    //ビューポート
    D3D12_VIEWPORT viewport = Argent::Graphics::Graphics::Instance()->GetViewport();

    //交換行列
    std::vector<GameObject*> _camera;
    GameObject::FindByTag(GameObject::Tag::MainCamera,_camera);
    Camera* camera = _camera.at(0)->GetComponent<Camera>();
    DirectX::XMMATRIX View = camera->GetViewMatrix();
    DirectX::XMMATRIX Projection = camera->GetProjectionMatrix();
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    //エネミー配置処理
    if (Argent::Input::Mouse::Instance().GetButtonDown(Argent::Input::Mouse::Button::RightButton))
    {
        DirectX::XMVECTOR startPosition = { Argent::Graphics::GetWindowWidth()/2,Argent::Graphics::GetWindowHeight() / 2,0.0f};
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
            meshResource,stage.at(0)->GetComponent<Argent::Component::Collider::RayCastCollider>()->GetWorldTransform(),
            result))
        {
            AddFriend(new FriendCreature(result.position));
        }
    }
}

BaseFriend* FriendManager::FindFriendComponentFromOwner(GameObject* wFriend) const
{
    for (auto& f : friendArray)
    {
        if (!f) continue;
        if (wFriend == f->GetOwner())
            return f;
    }

    return nullptr;
}