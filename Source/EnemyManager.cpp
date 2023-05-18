#include "EnemyManager.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"

bool EnemyManager::battleFlag = false;

void EnemyManager::Initialize()
{
    BaseActor::Initialize();
    GetOwner()->ReplaceTag(GameObject::Tag::EnemyManager);

    SetEnemyOnStage();
}

void EnemyManager::Update()
{
    for (auto activer = enemyArray.begin(); activer != enemyArray.end(); ++activer)
    {
        for (auto passiver = activer + 1; passiver != enemyArray.end(); ++passiver)
        {
            (*activer)->OnCollision((*passiver)->GetOwner()->GetComponent<Argent::Component::Collider::SphereCollider>());
        }
    }
}

void EnemyManager::DrawDebug()
{
    static DirectX::XMFLOAT3 pos{};
    static int type{};
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::SliderFloat3("SpawnPos", &pos.x, -100.0f, 100.0f);
        ImGui::SliderInt("EnemyType", &type, 0, static_cast<int>(Type::End) - 1);
        ImGui::Text("1: Spikebot\n2: Turret\n");
        if (ImGui::Button("AddEnemy"))
        {
            switch (type)
            {
            case static_cast<int>(Type::Spikebot):  // スパイクボット
                AddEnemy(new EnemySpikeBot(pos));
                break;
            case static_cast<int>(Type::Turret):
                AddEnemy(new EnemyTurret(pos));
                break;
            }
        }
        ImGui::TreePop();
    }
    BaseActor::DrawDebug();
}

void EnemyManager::AddEnemy(BaseEnemy* _enemy)
{
    GameObject::Instantiate("Enemy", _enemy);
    enemyArray.emplace_back(_enemy);

    // タグ登録はそれぞれのエネミー本体で行っている
}

void EnemyManager::SetEnemyOnStage()
{
    AddEnemy(new EnemySpikeBot(DirectX::XMFLOAT3(- 150, 0, 150)));
    AddEnemy(new EnemySpikeBot(DirectX::XMFLOAT3(150, 0, 150)));
    AddEnemy(new EnemySpikeBot(DirectX::XMFLOAT3(0, 0, 130)));

    AddEnemy(new EnemyTurret(DirectX::XMFLOAT3(0, 0, 56)));
    AddEnemy(new EnemyTurret(DirectX::XMFLOAT3(80, 0, 70)));
    AddEnemy(new EnemyTurret(DirectX::XMFLOAT3(-80, 0, 70)));
    AddEnemy(new EnemyTurret(DirectX::XMFLOAT3(150, 0, 90)));
    AddEnemy(new EnemyTurret(DirectX::XMFLOAT3(-150, 0, 90)));
}

BaseEnemy* EnemyManager::FindEnemyComponentFromOwner(GameObject* wEnemy) const
{
    for (auto& e : enemyArray)
    {
        if (!e)continue;
        if (wEnemy == e->GetOwner())
            return e;
    }

    return nullptr;
}
