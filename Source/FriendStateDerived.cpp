#include "FriendStateDerived.h"
#include "Argent/Argent.h"
#include "FriendCreature.h"
#include "FriendDrone.h"
#include "Character.h"

namespace Friend::Creature 
{
	void IdleState::Enter()
	{
		owner->SetAnimation(static_cast<int>(CreatureAnimation::Idle));

	}

	void IdleState::Execute()
	{
		float timer = owner->GetStateTimer();
		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
		if (!owner->SerchEnemy())return;
		if (!owner->IsAnimationEnd())return;
		
		if (owner->IsTargetInAttackArea())
		{
			if (owner->GetAttackTimer() > 0)return;
		}
		
		
		owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Action));
	}

	void IdleState::Exit()
	{
	}

	void ActionState::Enter()
	{
		owner->SetAnimation(static_cast<int>(CreatureAnimation::Action));
	}

	void ActionState::Execute()
	{
		if (!owner->IsAnimationEnd())return;

		//�G�����Ȃ�
		if (!owner->SerchEnemy())//TODO:���ƂŎ���
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Idle));
		}

		//�G���U���͈͓��ɂ��邩�ǂ���
		if (owner->IsTargetInAttackArea())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Attack));
			return;
		}
		else
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Walk));
		}


	}

	void ActionState::Exit()
	{
	}

	void WalkState::Enter()
	{
		owner->SetAnimation(static_cast<int>(CreatureAnimation::Walk_ChangeFrom_Action));
		//owner->SetStateTimer(10.0f);
	}

	void WalkState::Execute()
	{
		int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
		float timer = owner->GetStateTimer();
		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());

		switch (owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimation())
		{
		case static_cast<int>(CreatureAnimation::Walk_ChangeFrom_Action):
			if (owner->IsAnimationEnd())
			{

				owner->SetMaxSpeed(maxSpeed);
				owner->SetAnimation(static_cast<int>(CreatureAnimation::Walk));
			}
			break;

		case static_cast<int>(CreatureAnimation::Walk):

			if (!owner->GetTarget())
			{
				owner->SetAnimation(static_cast<int>(CreatureAnimation::Walk_End));
			}

			owner->MoveToTarget();

			//�������������Ă��邩��ړ����x�ɕω���t����
			if (animationFrame == startMovingFastFrame)
			{
				//owner->SetMaxSpeed(maxSpeed_fast);
				owner->SetAccelaration(acceleration);

			}
			if (animationFrame == endMovingFastFrame)
			{
				//owner->SetMaxSpeed(maxSpeed_late);
				owner->SetAccelaration(0.0f);
			}

			if (owner->GetTarget())
			{
				float vx = owner->GetTarget()->GetOwner()->GetTransform()->GetPosition().x - owner->GetOwner()->GetTransform()->GetPosition().x;
				float vz = owner->GetTarget()->GetOwner()->GetTransform()->GetPosition().z - owner->GetOwner()->GetTransform()->GetPosition().z;
				float length = sqrtf(vx * vx + vz * vz);
				if (length < owner->GetAttackAreaRadius())
				{
					owner->SetAccelaration(owner->Init_GetAccelaration());
					owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
					owner->SetAnimation(static_cast<int>(CreatureAnimation::Walk_End));
				}
			}

			break;

		case static_cast<int>(CreatureAnimation::Walk_End):
			if (owner->IsAnimationEnd())
			{
				owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Idle));
			}
			break;

		default:
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Idle));
			break;
		}

	}

	void WalkState::Exit()
	{
	}

	void AttackState::Enter()
	{
		owner->SetAnimation(static_cast<int>(CreatureAnimation::Attack));
	}

	void AttackState::Execute()
	{
		if (owner->IsAnimationEnd())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Idle));
		}

		int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
		if (animationFrame == efeStartFrame)
		{
			owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);

			if (owner->GetTarget()->ApplyDamage(owner->GetAttack()))
			{
				owner->HitStop(hitStopTime);
			}
			
			//�������d���Ƃ��Ŗ����ꂱ�̃L�[�t���[������΂��ꂽ�Ƃ��͍Ō�ɍU������������邽�߂Ƀt���O��p��
			didAttack = true;
		}
	}

	void AttackState::Exit()
	{
		if (!didAttack)
		{
			//�G�ւ̃_���[�W����

			
		}
		owner->SetAttackTimer(attackInterval);
	}

	void DieState::Enter()
	{
		owner->SetAccelaration(0);
		owner->SetMaxSpeed(0);
		owner->SetAnimation(static_cast<int>(CreatureAnimation::Die));
	}

	void DieState::Execute()
	{
		if (owner->IsAnimationEnd())
		{
			owner->GetOwner()->Destroy(owner->GetOwner());
		}
	}

	void DieState::Exit()
	{
	}
}

namespace Friend::Drone
{
	void IdleState::Enter()
	{
		
	}

	void IdleState::Execute()
	{
		float timer = owner->GetStateTimer();
		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
		if (!owner->SerchEnemy())return;

		if (owner->IsTargetInAttackArea())
		{
			if (owner->GetAttackTimer() > 0)return;
		}

		//�G�����Ȃ�
		if (!owner->GetTarget())
		{
			return;
		}

		//�G���U���͈͓��ɂ��邩�ǂ���
		if (owner->IsTargetInAttackArea())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Attack));
			return;
		}
		else
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Walk));
		}
	}

	void IdleState::Exit()
	{
	}

	void WalkState::Enter()
	{
		owner->SetMaxSpeed(maxSpeed);
		owner->SetAccelaration(acceleration);
	}

	void WalkState::Execute()
	{
		float timer = owner->GetStateTimer();
		if (!owner->SerchEnemy())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Idle));
		}

		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());

		owner->MoveToTarget();

		if (owner->GetTarget())
		{
			float vx = owner->GetTarget()->GetOwner()->GetTransform()->GetPosition().x - owner->GetOwner()->GetTransform()->GetPosition().x;
			float vz = owner->GetTarget()->GetOwner()->GetTransform()->GetPosition().z - owner->GetOwner()->GetTransform()->GetPosition().z;
			float length = sqrtf(vx * vx + vz * vz);
			if (length < owner->GetAttackAreaRadius())
			{
				owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Attack));
			}
		}
		else
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Idle));
		}
		
	}

	void WalkState::Exit()
	{
		owner->SetAccelaration(0.0f);
		owner->SetVelocity(DirectX::XMFLOAT3(0, 0, 0));
	}

	void AttackState::Enter()
	{
		if (owner->GetAttackTimer() > 0)owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Idle));
		owner->SetStateTimer(3.0f);

		
	}

	void AttackState::Execute()
	{
		float timer = owner->GetStateTimer();
		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
		if (owner->GetStateTimer() <= 0)
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Idle));
		}
		//TODO:�G�̃|�C���^�[���Ăяo��������
			//owner->GetTarget()->A(�G�ւ̃_���[�W����)
			// 
			//�������d���Ƃ��Ŗ����ꂱ�̃L�[�t���[������΂��ꂽ�Ƃ��͍Ō�ɍU������������邽�߂Ƀt���O��p��
		didAttack = true;
	}

	void AttackState::Exit()
	{
		if (!didAttack)
		{
			//�G�ւ̃_���[�W����
		}
		owner->SetAttackTimer(attackInterval);
	}
}

