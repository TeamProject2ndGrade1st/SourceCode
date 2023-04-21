#include "FriendStateDerived.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"

void IdleState::Enter()
{
    owner->SetAnimation(static_cast<int>(FriendAnimation::Idle)); 
	owner->SetStateTimer(10.0f);
	
}

void IdleState::Execute()
{
	float timer = owner->GetStateTimer();
	owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());
	if (timer < 0.0f)
	{
		if (owner->isAnimationEnd())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Action));
		}
	}
}

void IdleState::Exit()
{
}

void ActionState::Enter()
{
	owner->SetAnimation(static_cast<int>(FriendAnimation::Action));
}

void ActionState::Execute()
{
	if (!owner->isAnimationEnd())return;

	//ìGÇ™Ç¢Ç»Ç¢
	if (0)//TODO:Ç†Ç∆Ç≈é¿ëï
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Idle));
	}

	//ìGÇ™çUåÇîÕàÕì‡Ç…Ç¢ÇÈÇ©Ç«Ç§Ç©
	float vx = owner->GetTargetPosition().x - owner->GetOwner()->GetTransform()->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetOwner()->GetTransform()->GetPosition().z;
	float length = sqrtf(vx * vx + vz * vz);
	if (length < owner->GetAttackAreaRadius())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Attack));
		return;
	}
	else
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Walk));
	}

	
}

void ActionState::Exit()
{
}

void WalkState::Enter()
{
	owner->SetAnimation(static_cast<int>(FriendAnimation::Walk_ChangeFrom_Action));
	//owner->SetStateTimer(10.0f);
}

void WalkState::Execute()
{
	int animationFrame = static_cast<int>(owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimationFrame());
	//TODO:ë´Çà¯Ç´Ç∏ÇÈÇ∆Ç´frictionÇè„Ç∞Ç‹Ç≠ÇÈ
	float timer = owner->GetStateTimer();
	owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());

	switch (owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimation())
	{
	case static_cast<int>(FriendAnimation::Walk_ChangeFrom_Action):
		if (owner->isAnimationEnd())
		{
			
			owner->SetMaxSpeed(maxSpeed);
			owner->SetAnimation(static_cast<int>(FriendAnimation::Walk));
		}
		break;

	case static_cast<int>(FriendAnimation::Walk):
		
		owner->MoveToTarget();

		//ë´Çà¯Ç´Ç∏Ç¡ÇƒÇ¢ÇÈÇ©ÇÁà⁄ìÆë¨ìxÇ…ïœâªÇïtÇØÇÈ
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
				owner->SetAnimation(static_cast<int>(FriendAnimation::Walk_End));
			}
		}
		
		break;

	case static_cast<int>(FriendAnimation::Walk_End):
		if (owner->isAnimationEnd())
		{
			owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Idle));
		}
		break;

	default:
		owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Idle));
		break;
	}
	
}

void WalkState::Exit()
{
}

void AttackState::Enter()
{
	owner->SetAnimation(static_cast<int>(FriendAnimation::Attack));
}

void AttackState::Execute()
{
	if (owner->isAnimationEnd())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Idle));
	}
}

void AttackState::Exit()
{
}

//äKëwç\ë¢ÇÃÉXÉeÅ[ÉgÅiÇ¢ÇÁÇÒÇ©Ç‡Åj
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


