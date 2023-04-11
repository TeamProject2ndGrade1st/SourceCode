#include "Player.h"
#include "Argent/Argent.h"

Player::Player():
	BaseActor("Player")
{
}

void Player::Update()
{
	if(Argent::Input::GetKey(KeyCode::W))
	{
		Transform* t = GetTransform();
		auto p = t->GetPosition();
		p.z += 2;
		t->SetPosition(p);
	}
	BaseActor::Update();
}
