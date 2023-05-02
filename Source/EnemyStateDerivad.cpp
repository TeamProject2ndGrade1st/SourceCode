#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"

// �X�p�C�N�{�b�g
namespace Enemy::SpikeBot
{
    // �ҋ@�X�e�[�g
    void IdleState::Enter()
    {
        // �ҋ@�A�j���[�V�������Z�b�g
        owner->SetAnimation(static_cast<int>(SpikeBotAnimation::Idle));
        owner->SetStateTimer(5.0f);
    }

    void IdleState::Execute()
    {
        owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);

        // friend����������U���X�e�[�g�ɓ���
#if 1
        owner->SetFriend(owner->SearchFriend());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(SpikeBotAnimation::Attack));
        }
#else
        // �z��o�[�W���������
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


    // �U���X�e�[�g
    void AttackState::Enter()
    {
        // �U���A�j���[�V�������Z�b�g
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
                // friend������Ă��ăm�b�N�o�b�N��^����
#if 1
                auto f = owner->GetFriend();               
                f->AddImpulse(DirectX::XMFLOAT3(0, 0, -1000));
                // �G�t�F�N�g��
                owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);
#else
                // �z��o�[�W����
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

        // �U���̃A�j���[�V�������I�������ҋ@�ɖ߂�
        if (owner->IsAnimationEnd())
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(SpikeBotAnimation::Idle));
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

    }

    void IdleState::Execute()
    {

    }

    void IdleState::Exit()
    {

    }

    // �U���X�e�[�g
    void AttackState::Enter()
    {

    }

    void AttackState::Execute()
    {

    }

    void AttackState::Exit()
    {

    }

    // �N���X�e�[�g
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