#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"

// スパイクボット
namespace Enemy::SpikeBot
{
    // 待機ステート
    void IdleState::Enter()
    {
        // 待機アニメーションをセット
        owner->SetAnimation(static_cast<int>(SpikeBotAnimation::Idle));
        owner->SetStateTimer(5.0f);
    }

    void IdleState::Execute()
    {
        owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);

        // friendを見つけたら攻撃ステートに入る
#if 1
        owner->SetFriend(owner->SearchFriend());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(SpikeBotAnimation::Attack));
        }
#else
        // 配列バージョンを作る
        owner->SearchFriendSet();
        if (owner->_friendArray.size() != 0)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(SpikeBotAnimation::Attack));
            // owner->_friendArray.clear();
        }
#endif
    }

    void IdleState::Exit()
    {

    }


    // 攻撃ステート
    void AttackState::Enter()
    {
        // 攻撃アニメーションをセット
        owner->SetAnimation(static_cast<int>(SpikeBotAnimation::Attack));
        once = false;
    }

    void AttackState::Execute()
    {
        owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);

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
            owner->GetStateMachine()->ChangeState(static_cast<int>(SpikeBotAnimation::Idle));
        }
    }

    void AttackState::Exit()
    {
    }
}

// タレット
namespace Enemy::Turret
{
    // 待機ステート
    void IdleState::Enter()
    {

    }

    void IdleState::Execute()
    {

    }

    void IdleState::Exit()
    {

    }

    // 攻撃ステート
    void AttackState::Enter()
    {

    }

    void AttackState::Execute()
    {

    }

    void AttackState::Exit()
    {

    }

    // 起動ステート
    void StartUpState::Enter()
    {

    }

    void StartUpState::Execute()
    {

    }

    void StartUpState::Exit()
    {

    }
}