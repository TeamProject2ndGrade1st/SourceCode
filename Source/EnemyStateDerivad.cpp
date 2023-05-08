#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"
#include "EnemyTurretShot.h"

// �X�p�C�N�{�b�g
namespace Enemy::SpikeBot
{
    // �ҋ@�X�e�[�g
    void IdleState::Enter()
    {
        // �ҋ@�A�j���[�V�������Z�b�g
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

    // �U���X�e�[�g
    void AttackState::Enter()
    {
        // �U���A�j���[�V�������Z�b�g
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

        // �U���̃A�j���[�V�������I�������ҋ@�ɖ߂�
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySpikeBot::State::Idle));
        }
    }

    void AttackState::Exit()
    {
    }
}

// �^���b�g
namespace Enemy::Turret
{
    // �ҋ@�X�e�[�g
    void IdleState::Enter()
    {
        // �A�j���[�V�����ݒ�
        owner->SetAnimation(static_cast<int>(TurretAnimation::Idle));

        // �^�C�}�[��ݒ�
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

    // �U���X�e�[�g
    void AttackState::Enter()
    {
        // �A�j���[�V�����ݒ�
        owner->SetAnimation(static_cast<int>(TurretAnimation::Attack));

        // �^�C�}�[��ݒ�
        owner->SetStateTimer(4.0f);

        DirectX::XMFLOAT3 pos{ owner->GetOwner()->GetTransform()->GetPosition() };
        GameObject::Instantiate("shot", new EnemyTurretShot("shot", pos));
    }

    void AttackState::Execute()
    {
        float stateTimer = owner->GetStateTimer();
        owner->SetStateTimer(stateTimer -= Argent::Timer::GetDeltaTime());

        if (stateTimer < 0.0f&&owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::Idle));
        }
    }

    void AttackState::Exit()
    {

    }

    // �N���X�e�[�g
    void StartUpState::Enter()
    {
        // �A�j���[�V�����ݒ�
        owner->SetAnimation(static_cast<int>(TurretAnimation::StartUp));
    }

    void StartUpState::Execute()
    {
        // �N���A�j���[�V�������I�������ҋ@��
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyTurret::State::Idle));
        }
    }

    void StartUpState::Exit()
    {

    }

    // �N���ҋ@�X�e�[�g
    void BootWaitState::Enter()
    {
        // �A�j���[�V�����ݒ�
        owner->SetAnimation(static_cast<int>(TurretAnimation::BootWait));
        
        // �^�C�}�[���Z�b�g
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