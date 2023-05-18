#include "EnemyTurretShotManager.h"
#include "EnemyTurretShot.h"

std::vector<EnemyTurretShot*> shotArray;

void EnemyTurretShotManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::TurretShotManager);
}

void EnemyTurretShotManager::Update()
{
    for (auto it = shotArray.begin(); it != shotArray.end();)
    {
        if ((*it)->eraseTimer <= 0.0f)
        {
            GameObject::Destroy((*it)->GetOwner());
            it = shotArray.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EnemyTurretShotManager::DrawDebug()
{
}

// shot‚ð¶¬‚·‚é
void EnemyTurretShotManager::AddShot(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3 targetPos)
{
    // shot‚ðì‚é
    EnemyTurretShot* shot = new EnemyTurretShot("shot", pos);
    shot->SetVelocity(targetPos);
    GameObject::Instantiate("shot", shot);

    DirectX::XMVECTOR front = { 0,0,-1 };
    DirectX::XMVECTOR axis = { 0,1,0 };
    DirectX::XMVECTOR right = { -1,0,0 };

    float angle = DirectX::XMVectorGetX(DirectX::XMVector3Dot(front, DirectX::XMLoadFloat3(&targetPos)));
    float LR = DirectX::XMVectorGetX(DirectX::XMVector3Dot(right, DirectX::XMLoadFloat3(&targetPos)));
    

    if (angle < 0.99f)
    {
        angle = acosf(angle);
        if (LR < 0)angle *= -1;

        shot->GetOwner()->GetTransform()->SetRotation({ 0,DirectX::XMConvertToDegrees(angle),0,0 });
    }    
    
    // shot‚ð”z—ñ‚É“o˜^‚·‚é
    shotArray.emplace_back(shot);
}
