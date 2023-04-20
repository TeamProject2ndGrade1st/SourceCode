#include "spikeBot.h"
#include "Argent/Argent.h"
#include "Argent/Graphic/Graphics.h"
#include "Argent/Input/Mouse.h"

// �R���X�g���N�^
SpikeBot::SpikeBot() :BaseActor("spikeBot")
{
}

void SpikeBot::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/spike_bot_0419_1.fbx"));
    GetOwner()->GetTransform()->SetScaleFactor(0.01f);
    GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
}

// �X�V
void SpikeBot::Update()
{
}

void SpikeBot::DrawDebug()
{
}
