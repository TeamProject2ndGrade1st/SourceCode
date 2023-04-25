#include "FriendStateDerived.h"
#include "Argent/Argent.h"
#include "FriendCreature.h"
#include "FriendDrone.h"

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

		//“G‚ª‚¢‚È‚¢
		if (!owner->SerchEnemy())//TODO:‚ ‚Æ‚ÅŽÀ‘•
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendCreature::State::Idle));
		}

		//“G‚ªUŒ‚”ÍˆÍ“à‚É‚¢‚é‚©‚Ç‚¤‚©
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
		owner->AddImpulse(DirectX::XMFLOAT3(0,0,20));
	}

	void WalkState::Execute()
	{
		int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
		//TODO:‘«‚ðˆø‚«‚¸‚é‚Æ‚«friction‚ðã‚°‚Ü‚­‚é
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

			owner->MoveToTarget();

			//‘«‚ðˆø‚«‚¸‚Á‚Ä‚¢‚é‚©‚çˆÚ“®‘¬“x‚É•Ï‰»‚ð•t‚¯‚é
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

			{
				float vx = owner->GetTargetPosition().x - owner->GetOwner()->GetTransform()->GetPosition().x;
				float vz = owner->GetTargetPosition().z - owner->GetOwner()->GetTransform()->GetPosition().z;
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
	}

	void AttackState::Exit()
	{
		owner->SetAttackTimer(attackInterval);
	}

	//ŠK‘w\‘¢‚ÌƒXƒe[ƒgi‚¢‚ç‚ñ‚©‚àj
	void H_MoveState::Enter()
	{
	}

	void H_MoveState::Execute()
	{
	}

	void H_MoveState::Exit()
	{
	}

	void H_BattleState::Enter()
	{
	}

	void H_BattleState::Execute()
	{
	}

	void H_BattleState::Exit()
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

		//“G‚ª‚¢‚È‚¢
		if (0)//TODO:‚ ‚Æ‚ÅŽÀ‘•
		{
			return;
		}

		//“G‚ªUŒ‚”ÍˆÍ“à‚É‚¢‚é‚©‚Ç‚¤‚©
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

		float vx = owner->GetTargetPosition().x - owner->GetOwner()->GetTransform()->GetPosition().x;
		float vz = owner->GetTargetPosition().z - owner->GetOwner()->GetTransform()->GetPosition().z;
		float length = sqrtf(vx * vx + vz * vz);
		if (length < owner->GetAttackAreaRadius())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Attack));
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

		//owner->GetOwner()->GetComponent<Argent::Component::Renderer::EffekseerEmitter>()->OnPlay(0);
	}

	void AttackState::Execute()
	{
		float timer = owner->GetStateTimer();
		owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
		if (owner->GetStateTimer() <= 0)
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(FriendDrone::State::Idle));
		}
	}

	void AttackState::Exit()
	{
		owner->SetAttackTimer(attackInterval);
	}
}

