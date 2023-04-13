#pragma once
#include "Argent/Argent.h"

class Player:
	public Argent::Component::BaseActor
{
public:
	Player();
	virtual ~Player() override = default;

	void Update() override;
	void Initialize() override;

#ifdef _DEBUG
	void DrawDebug() override;
#endif


	void OnRayCollision(const Argent::Component::Collider::RayCastCollider* other) override;
private:
	int state = 0;

	GameObject* camera;

	float movement = 1.0f;

	void MoveCamera();

	Argent::Component::Collision::RayCast* ray;
protected:

};

