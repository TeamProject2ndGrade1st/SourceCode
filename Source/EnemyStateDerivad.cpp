#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseEnemy.h"
#include "BaseFriend.h"


void EnemyIdleState::Enter()
{
    // 待機アニメーションをセット
    owner->SetAnimation(static_cast<int>(EnemyAnimation::Idle));
    owner->SetStateTimer(5.0f);
}

void EnemyIdleState::Execute()
{
    // タイマーが０になったら攻撃に移る
    //float timer = owner->GetStateTimer();
    //owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
    //if (timer < 0.0f)
    //{
    //    if (owner->isAnimationEnd())
    //    {
    //        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
    //    }
    //}
    if (SearchFriend())
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
    }
}

void EnemyIdleState::Exit()
{

}

bool EnemyIdleState::SearchFriend()
{
    // Friendタグが付いているGameObjectを探す
    std::vector<GameObject*> Friend;
    GameObject::FindByTag(GameObject::Tag::Friend, Friend);

    DirectX::XMFLOAT3 pos = owner->GetOwner()->GetTransform()->GetPosition();
    DirectX::XMFLOAT4 angle = owner->GetOwner()->GetTransform()->GetRotation();

    // TO DO 途中
    for (auto f : Friend)
    {
        // Friend の positionを取る
        DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

        float vx = pos.x - friendPos.x;
        float vy = pos.y - friendPos.y;
        float vz = pos.z - friendPos.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < searchRange)
        {
            float distXZ = sqrtf(vx * vx + vz * vz);
            // 単位ベクトル化
            vx /= distXZ;
            vz /= distXZ;

            // 方向ベクトル化
            float frontX = sinf(angle.y);
            float frontZ = cosf(angle.y);
            // 2つのベクトルの内積値で前後判定
            float dot = (frontX * vx) + (frontZ * vz);
            if (dot > 0.0f)
            {
                return true;
            }
        }
    }
    return false;
}

void EnemyAttackState::Enter()
{
    // 攻撃アニメーションをセット
    owner->SetAnimation(static_cast<int>(EnemyAnimation::Attack));
}

void EnemyAttackState::Execute()
{
    // 攻撃のアニメーションが終わったら待機に戻る
    if (owner->isAnimationEnd())
    {
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Idle));
    }
}

void EnemyAttackState::Exit()
{
}
