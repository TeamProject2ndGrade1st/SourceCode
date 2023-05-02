#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseEnemy.h"
#include "BaseFriend.h"

namespace Enemy::SpikeBot
{
    void IdleState::Enter()
    {
        // �ҋ@�A�j���[�V�������Z�b�g
        owner->SetAnimation(static_cast<int>(EnemyAnimation::Idle));
        owner->SetStateTimer(5.0f);
    }

    void IdleState::Execute()
    {
        //owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);
        // �^�C�}�[���O�ɂȂ�����U���Ɉڂ�
        //float timer = owner->GetStateTimer();
        //owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
        //if (timer < 0.0f)
        //{
        //    if (owner->isAnimationEnd())
        //    {
        //        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
        //    }
        //}
        //if (SearchFriend())

        //owner->addspeed();

        
        

        
        
        owner->SetFriend(owner->SearchFriend1());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
        }
    }

    void IdleState::Exit()
    {

    }



    void AttackState::Enter()
    {
        // �U���A�j���[�V�������Z�b�g
        owner->SetAnimation(static_cast<int>(EnemyAnimation::Attack));
        once = false;
    }

    void AttackState::Execute()
    {
        owner->addspeed();

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

        // �U���̃A�j���[�V�������I�������ҋ@�ɖ߂�
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