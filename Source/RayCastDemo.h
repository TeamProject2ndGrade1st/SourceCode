#pragma once
#include "Argent/Argent.h"

class RayCastDemo:
	public Argent::Component::BaseActor
{
public:
	RayCastDemo():
		BaseActor("RayCastDemo")
	{}

	void Initialize() override;
	
};
