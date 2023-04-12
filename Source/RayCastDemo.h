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

	void OnRayCollision(const Argent::Component::Collider::RayCastCollider* other) override;
	void Update() override;
	void DrawDebug() override;

	Argent::Component::Collision::RayCast* ray;
	float moveSpeed;
};
