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

// shot‚ğ¶¬‚·‚é
void EnemyTurretShotManager::AddShot(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT3 targetPos)
{
    // shot‚ğì‚é
    EnemyTurretShot* shot = new EnemyTurretShot("shot", pos);
    shot->SetVelocity(targetPos);
    GameObject::Instantiate("shot", shot);
    
    // shot‚Ìi‚Ş•ûŒü‚ğİ’è‚·‚é
    
    
    // shot‚ğ”z—ñ‚É“o˜^‚·‚é
    shotArray.emplace_back(shot);
}
