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

	void Update() override;
#ifdef _DEBUG
	void DrawDebug() override;
#endif
	Argent::Component::Collision::RayCast* ray;
	float moveSpeed;
};
