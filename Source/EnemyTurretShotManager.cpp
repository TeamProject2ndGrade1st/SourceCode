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

void EnemyTurretShotManager::AddShot(const DirectX::XMFLOAT3 pos)
{
    EnemyTurretShot* shot = new EnemyTurretShot("shot", pos);
    GameObject::Instantiate("shot", shot);
    shotArray.emplace_back(shot);
    // TO DO ‚±‚±‰ðŒˆ‚µ‚È‚¢‚Æ‚¢‚¯‚È‚¢
}
