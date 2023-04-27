#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseEnemy.h"
#include "BaseFriend.h"

namespace Enemy::SpikeBot
{
    void IdleState::Enter()
    {
        // 待機アニメーションをセット
        owner->SetAnimation(static_cast<int>(EnemyAnimation::Idle));
        owner->SetStateTimer(5.0f);
    }

    void IdleState::Execute()
    {
        // friendを見つけたら攻撃ステートに入る
#if 1
        owner->SetFriend(owner->SearchFriend());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
        }
#else
        // 配列バージョンを作る
        owner->SearchFriendSet();
        if (owner->_friendArray.size() != 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
            // owner->_friendArray.clear();
        }
#endif
    }

    void IdleState::Exit()
    {

    }

#if 1
    bool IdleState::SearchFriend()
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
                    f->GetTransform()->SetPosition(friendPos);

                    return true;
                }
            }
        }
        return false;
    }

    BaseFriend* IdleState::SearchFriend1()
    {
        // TODO: return ステートメントをここに挿入します
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
                    f->GetTransform()->SetPosition(friendPos);
                    return reinterpret_cast<BaseFriend*> (f);
                    //return f;
                }
            }
        }
        return nullptr;
    }
#endif


    void AttackState::Enter()
    {
        // 攻撃アニメーションをセット
        owner->SetAnimation(static_cast<int>(EnemyAnimation::Attack));
        once = false;
    }

    void AttackState::Execute()
    {
        if (!once)
        {
            //int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
            float anime = owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame();

            if (anime >= attackTime)
            {
                // friendを取ってきてノックバックを与える
#if 1
                auto f = owner->GetFriend();               
                f->AddImpulse(DirectX::XMFLOAT3(0, 0, -1000));
                // エフェクト仮
                owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);
#else
                // 配列バージョン
                auto friend_array = owner->GetFriendArray();
                for (auto& f : friend_array)
                {
                    f->AddImpulse(DirectX::XMFLOAT3(0, 0, -1000));
                }
                friend_array.clear();
#endif
                once = true;
            }
        }
        //owner->AddImpulse()

        // 攻撃のアニメーションが終わったら待機に戻る
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Idle));
        }
    }

    void AttackState::Exit()
    {
    }
}

namespace Enemy::Turret
{
    void IdleState::Enter()
    {

    }

    void IdleState::Execute()
    {

    }

    void IdleState::Exit()
    {

    }
}