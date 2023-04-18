#include "StateDerived.h"
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
			owner->GetStateMachine()->ChangeState(static_cast<int>(BaseFriend::State::Walk));
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

	if (true)
	{

	}
}

void ActionState::Exit()
{
}

void WalkState::Enter()
{
	owner->SetAnimation(static_cast<int>(FriendAnimation::Action));
	owner->SetStateTimer(10.0f);
}

void WalkState::Execute()
{
	float timer = owner->GetStateTimer();
	owner->SetStateTimer(timer -= Argent::Timer::GetDeltaTime());

	switch (owner->GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->GetAnimation())
	{
	case static_cast<int>(FriendAnimation::Action):
		if (owner->isAnimationEnd())
		{
			owner->SetAnimation(static_cast<int>(FriendAnimation::Walk_ChangeFrom_Action));
		}
		break;

	case static_cast<int>(FriendAnimation::Walk_ChangeFrom_Action):
		if (owner->isAnimationEnd())
		{
			owner->SetAnimation(static_cast<int>(FriendAnimation::Walk));
		}
		break;

	case static_cast<int>(FriendAnimation::Walk):
		if (timer < 0.0f)
		{
			owner->SetAnimation(static_cast<int>(FriendAnimation::Walk_End));
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
}

void AttackState::Execute()
{
}

void AttackState::Exit()
{
}

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


