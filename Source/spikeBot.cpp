#include "spikeBot.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"

// コンストラクタ
SpikeBot::SpikeBot() :BaseActor("spikeBot")
{
}

void SpikeBot::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike_bot_0419_1.fbx"));
    GetOwner()->GetTransform()->SetScaleFactor(0.1f);
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));

    // 仮のタグ
    GetOwner()->SetTag(GameObject::Tag::Friend);
    // 仮
    DirectX::XMFLOAT3 pos{ 0,0,-50 };
    GetOwner()->GetTransform()->SetPosition(pos);
}

// 更新
void SpikeBot::Update()
{
}

void SpikeBot::DrawDebug()
{
}
