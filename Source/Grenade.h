#pragma once
#include "Argent/Argent.h"

class Grenade:
	public Argent::Component::BaseActor
{
public:
	Grenade():
		BaseActor("Grenade")
	{};

	~Grenade() override = default;

	void Initialize() override;



};

