#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"
#include "EnemyTurretShot.h"
#include "EnemyTurretShotManager.h"

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
                
                f->ApplyDamage(owner->GetAttack());

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
        owner->SetStateTimer(10.0f);
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
        
        // �V���b�g�^�C�}�[���Z�b�g
        shotTimer = 0.0f;

        std::vector<GameObject*>chiledArray;
        owner->GetOwner()->GetChildArray(chiledArray);
        for (auto& c : chiledArray)
        {
            if (c->GetName() == "polySurface1")
            {
                head = c;
                break;
            }
        }
    }

    void AttackState::Execute()
    {
        DirectX::XMFLOAT3 tPos{};
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
            if (Friend.size()!=0)
            {
                float length0 = FLT_MAX;
                int  num = 0;
                int i = 0;
                for (auto it = Friend.begin(); it != Friend.end(); ++it,++i)
                {
                    if ((*it)->GetIsActive())continue;
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
                        num = i;
                    }                    
                }
                
                auto f_riend = fManager->friendArray.at(num);
                f_riend->ApplyDamage(owner->GetAttack());
            }

            // �^���b�g�̈ʒu�����
            DirectX::XMFLOAT3 pos{ owner->GetOwner()->GetTransform()->GetPosition() };
            // �e�𐶐�����
            EnemyTurretShotManager::Instance().AddShot(pos, tPos);
            // �_���[�W�����
            

            shotTimer = 0.4f;
        }
        shotTimer -= Argent::Timer::GetDeltaTime();

        // todo����
        if (head->GetName() == "polySurface1")
        {
            DirectX::XMFLOAT3 f = head->GetTransform()->CalcForward();
            DirectX::XMVECTOR front = DirectX::XMLoadFloat3(&f);
            DirectX::XMVECTOR axis = { 0,1,0 };
            DirectX::XMFLOAT3 r = head->GetTransform()->CalcRight();
            DirectX::XMVECTOR right = DirectX::XMLoadFloat3(&r);

            float angle = DirectX::XMVectorGetX(DirectX::XMVector3Dot(front, DirectX::XMLoadFloat3(&tPos)));
            float LR = DirectX::XMVectorGetX(DirectX::XMVector3Dot(right, DirectX::XMLoadFloat3(&tPos)));

            if (angle < 0.99f)
            {
                angle = acosf(angle);
                if (LR < 0)angle *= -1; 

                auto angleY = head->GetTransform()->GetRotation().y;
                head->GetTransform()->SetRotation({ 0,DirectX::XMConvertToDegrees(angle) +angleY,0,0 });
            }
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