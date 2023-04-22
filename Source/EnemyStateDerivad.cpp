#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseEnemy.h"
#include "BaseFriend.h"


void EnemyIdleState::Enter()
{
    owner->SetAnimation(static_cast<int>(EnemyAnimation::Idle));
    owner->SetStateTimer(5.0f);
}

void EnemyIdleState::Execute()
{
    float timer = owner->GetStateTimer();
    owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
    if (timer < 0.0f)
    {
        if (owner->isAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
        }
    }
}

void EnemyIdleState::Exit()
{

}

bool EnemyIdleState::SearchPlayer()
{
    // タグを追加する
    owner->GetOwner()->SetTag(GameObject::Tag::SpikeBot);
    
    //GameObject::FindByTag()



    // プレイヤーとの高低差を考慮して3Dで距離判定をする
    //const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    //float vx = playerPosition.x - position.x;
    //float vy = playerPosition.y - position.y;
    //float vz = playerPosition.z - position.z;
    //float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    //if (dist < searchRange)
    //{
    //	float distXZ = sqrtf(vx * vx + vz * vz);
    //	// 単位ベクトル化
    //	vx /= distXZ;
    //	vz /= distXZ;

    //	// 方向ベクトル化
    //	float frontX = sinf(angle.y);
    //	float frontZ = cosf(angle.y);
    //	// 2つのベクトルの内積値で前後判定
    //	float dot = (frontX * vx) + (frontZ * vz);
    //	if (dot > 0.0f)
    //	{
    //		return true;
    //	}
    //}
    return false;
}

void EnemyAttackState::Enter()
{
    owner->SetAnimation(static_cast<int>(EnemyAnimation::Attack));
}

void EnemyAttackState::Execute()
{
}

void EnemyAttackState::Exit()
{
}
