#include "muzzleFlash.h"

void Shadow::Initialize()
{
    BaseActor::Initialize();

    GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/shadow0425_3.fbx", false));

    //�n�ʂ���������ɂ���
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.1f, pos.z));

    GetTransform()->SetScaleFactor(parent->GetOwner()->GetTransform()->GetScaleFactor());
}

void Shadow::Update()
{
    //�e�I�u�W�F�N�g�̍��W������
    DirectX::XMFLOAT3 pos = parent->GetOwner()->GetTransform()->GetPosition();
    GetTransform()->SetPosition(DirectX::XMFLOAT3(pos.x, 0.01f, pos.z));
    GetTransform()->SetScaleFactor(parent->GetOwner()->GetTransform()->GetScaleFactor());
}
