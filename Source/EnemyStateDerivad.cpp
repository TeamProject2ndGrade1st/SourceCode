#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"
#include "EnemyTurretShot.h"
#include "EnemyTurretShotManager.h"

// スパイクボット
namespace Enemy::SpikeBot
{
    // 待機ステート
    void IdleState::Enter()
    {
        // 待機アニメーションをセット
        owner->SetAnimation(static_cast<int>(SpikeBotAnimation::Idle));
        //owner->SetStateTimer(5.0f);
    }

    void IdleState::Execute()
    {
        owner->SetFriend(owner->SearchFriend1());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySpikeBot::State::Attack));
        }
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
        

        if (!once)
        {
            //int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
            float anime = owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame();

            if (anime >= attackTime)
            {
                auto f = owner->GetFriend();
                
                //owner->friendAddSpeed = 3;
                
                
                f->AddImpulse(DirectX::XMFLOAT3(0, 0, -1000));
                owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);
                

                once = true;
            }
        }
        //owner->AddImpulse()

        // 攻撃のアニメーションが終わったら待機に戻る
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySpikeBot::State::Idle));
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
        // アニメーション設定
        owner->SetAnimation(static_cast<int>(TurretAnimation::Idle));

        // タイマーを設定
        owner->SetStateTimer(3.0f);
    }

    void IdleState::Execute()
    {
        float stateTimer = owner->GetStateTimer();
        owner->SetStateTimer(stateTimer -= Argent::Timer::GetDeltaTime());

        if (stateTimer < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::Attack));
        }
    }

    void IdleState::Exit()
    {

    }

    // 攻撃ステート
    void AttackState::Enter()
    {
        // アニメーション設定
        owner->SetAnimation(static_cast<int>(TurretAnimation::Attack));

        // タイマーを設定
        owner->SetStateTimer(4.0f);
        
        // ショットタイマーをセット
        shotTimer = 0.0f;


    }

    void AttackState::Execute()
    {
        float stateTimer = owner->GetStateTimer();
        owner->SetStateTimer(stateTimer -= Argent::Timer::GetDeltaTime());

        if (stateTimer < 0.0f&&owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::Idle));
        }

        if (shotTimer <= 0.0f)
        {
            // test
            DirectX::XMFLOAT3 targetPos{};
            
            std::vector<GameObject*> Friend;
            GameObject::FindByTag(GameObject::Tag::Friend, Friend);
            auto fManager = GameObject::FindByName("FriendManager")->GetComponent<FriendManager>();
            //fManager->FindFriendComponentFromOwner()
            
            //Friend.at(0);
            //owner->SetFriend(owner->SearchFriend1());
            DirectX::XMFLOAT3 tPos{};
            if (Friend.size()!=0)
            {
                float length0 = FLT_MAX;
                for (auto it = Friend.begin(); it != Friend.end(); ++it)
                {
                    DirectX::XMFLOAT3 friendPos = (*it)->GetTransform()->GetPosition();
                    //DirectX::XMFLOAT3 friendPos = owner->_friend->GetTargetPosition();
                    DirectX::XMFLOAT3 turretPos = owner->GetOwner()->GetTransform()->GetPosition();
                    DirectX::XMVECTOR fv = DirectX::XMLoadFloat3(&friendPos);
                    DirectX::XMVECTOR tv = DirectX::XMLoadFloat3(&turretPos);
                    DirectX::XMVECTOR v = DirectX::XMVectorSubtract(fv, tv);
                    DirectX::XMVECTOR Length = DirectX::XMVector3Length(v);
                    float length;
                    DirectX::XMStoreFloat(&length, Length);

                    if (length < length0)
                    {
                        v = DirectX::XMVector3Normalize(v);
                        DirectX::XMStoreFloat3(&targetPos, v);
                        length0 = length;
                        tPos = targetPos;
                    }
                }
            }

            // タレットの位置を取る
            DirectX::XMFLOAT3 pos{ owner->GetOwner()->GetTransform()->GetPosition() };
            // 弾を生成する
            EnemyTurretShotManager::Instance().AddShot(pos, tPos);
            shotTimer = 0.4f;
        }
        shotTimer -= Argent::Timer::GetDeltaTime();

    }

    void AttackState::Exit()
    {

    }

    // 起動ステート
    void StartUpState::Enter()
    {
        // アニメーション設定
        owner->SetAnimation(static_cast<int>(TurretAnimation::StartUp));
    }

    void StartUpState::Execute()
    {
        // 起動アニメーションが終わったら待機へ
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::Idle));
        }
    }

    void StartUpState::Exit()
    {

    }

    // 起動待機ステート
    void BootWaitState::Enter()
    {
        // アニメーション設定
        owner->SetAnimation(static_cast<int>(TurretAnimation::BootWait));
        
        // タイマーをセット
        owner->SetStateTimer(10.0f);
    }

    void BootWaitState::Execute()
    {
        float stateTimer = owner->GetStateTimer();
        owner->SetStateTimer(stateTimer -= Argent::Timer::GetDeltaTime());

        if (stateTimer < 0.0f)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::StartUp));
        }
    }

    void BootWaitState::Exit()
    {

    }
}