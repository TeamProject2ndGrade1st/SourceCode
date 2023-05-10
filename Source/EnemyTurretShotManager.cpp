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

// shotを生成する
void EnemyTurretShotManager::AddShot(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3 targetPos)
{
    // shotを作る
    EnemyTurretShot* shot = new EnemyTurretShot("shot", pos);
    shot->SetVelocity(targetPos);
    GameObject::Instantiate("shot", shot);
    
    // shotの進む方向を設定する
    
    
    // shotを配列に登録する
    shotArray.emplace_back(shot);
}
