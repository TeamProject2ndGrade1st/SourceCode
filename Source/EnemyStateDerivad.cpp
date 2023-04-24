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
        
        owner->SetFriend(SearchFriend1());
        if (owner->_friend != nullptr)
        {
            owner->GetStateMachine()->ChangeState(static_cast<int>(EnemyAnimation::Attack));
        }
    }

    void IdleState::Exit()
    {

    }

    bool IdleState::SearchFriend()
    {
        // Friend�^�O���t���Ă���GameObject��T��
        std::vector<GameObject*> Friend;
        GameObject::FindByTag(GameObject::Tag::Friend, Friend);

        DirectX::XMFLOAT3 pos = owner->GetOwner()->GetTransform()->GetPosition();
        DirectX::XMFLOAT4 angle = owner->GetOwner()->GetTransform()->GetRotation();

        // TO DO �r��
        for (auto f : Friend)
        {
            // Friend �� position�����
            DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

            float vx = pos.x - friendPos.x;
            float vy = pos.y - friendPos.y;
            float vz = pos.z - friendPos.z;
            float dist = sqrtf(vx * vx + vy * vy + vz * vz);

            if (dist < searchRange)
            {
                float distXZ = sqrtf(vx * vx + vz * vz);
                // �P�ʃx�N�g����
                vx /= distXZ;
                vz /= distXZ;

                // �����x�N�g����
                float frontX = sinf(angle.y);
                float frontZ = cosf(angle.y);
                // 2�̃x�N�g���̓��ϒl�őO�㔻��
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
        // TODO: return �X�e�[�g�����g�������ɑ}�����܂�
        // Friend�^�O���t���Ă���GameObject��T��
        std::vector<GameObject*> Friend;
        GameObject::FindByTag(GameObject::Tag::Friend, Friend);

        DirectX::XMFLOAT3 pos = owner->GetOwner()->GetTransform()->GetPosition();
        DirectX::XMFLOAT4 angle = owner->GetOwner()->GetTransform()->GetRotation();

        // TO DO �r��
        for (auto f : Friend)
        {
            // Friend �� position�����
            DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

            float vx = pos.x - friendPos.x;
            float vy = pos.y - friendPos.y;
            float vz = pos.z - friendPos.z;
            float dist = sqrtf(vx * vx + vy * vy + vz * vz);

            if (dist < searchRange)
            {
                float distXZ = sqrtf(vx * vx + vz * vz);
                // �P�ʃx�N�g����
                vx /= distXZ;
                vz /= distXZ;

                // �����x�N�g����
                float frontX = sinf(angle.y);
                float frontZ = cosf(angle.y);
                // 2�̃x�N�g���̓��ϒl�őO�㔻��
                float dot = (frontX * vx) + (frontZ * vz);
                if (dot > 0.0f)
                {
                    f->GetTransform()->SetPosition(friendPos);
                    return reinterpret_cast<BaseFriend*>(f);
                    //return f;
                }
            }
        }
        return nullptr;
    }


    void AttackState::Enter()
    {
        // �U���A�j���[�V�������Z�b�g
        owner->SetAnimation(static_cast<int>(EnemyAnimation::Attack));
        once = false;
    }

    void AttackState::Execute()
    {
        if (!once)
        {
            int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());

            if (animationFrame == attackTime)
            {
                auto f = owner->GetFriend();
                DirectX::XMFLOAT3 friendPos = f->GetOwner()->GetTransform()->GetPosition();
                friendPos.z -= 10;
                f->GetOwner()->GetTransform()->SetPosition(friendPos);
                
                
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