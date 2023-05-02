#include "muzzleFlash.h"

void Shadow::Initialize()
{
    BaseActor::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/shadow0425_3.fbx", false));

    //地面よりも少し上にする
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.1f, pos.z));

    GetTransform()->SetScaleFactor(parent->GetOwner()->GetTransform()->GetScaleFactor());
}

void Shadow::Update()
{
    //親オブジェクトの座標を入れる
    DirectX::XMFLOAT3 pos = parent->GetOwner()->GetTransform()->GetPosition();
    GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.01f, pos.z));
    GetTransform()->SetScaleFactor(parent->GetOwner()->GetTransform()->GetScaleFactor());
}
