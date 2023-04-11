#pragma once
#include "Argent/Argent.h"

class Player:
	public Argent::Component::BaseActor
{
public:
	Player();
	virtual ~Player() override = default;

	void Update() override;
protected:

};

