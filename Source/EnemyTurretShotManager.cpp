#include "EnemyTurretShotManager.h"
#include "EnemyTurretShot.h"

void EnemyTurretShotManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::TurretShotManager);
}

void EnemyTurretShotManager::Update()
{
    for (auto& shot : shotArray)
    {
        if (shot->eraseTimer <= 0.0f)
        {
            delete shot;
            
        }
    }
}

void EnemyTurretShotManager::DrawDebug()
{
}

void EnemyTurretShotManager::AddShot(const DirectX::XMFLOAT3 pos)
{
    EnemyTurretShot* shot = new EnemyTurretShot("shot", pos);
    GameObject::Instantiate("shot", shot);
    shotArray.emplace_back(shot);
    // TO DO ‚±‚±‰ğŒˆ‚µ‚È‚¢‚Æ‚¢‚¯‚È‚¢
}
