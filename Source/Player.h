#pragma once
#include "Argent/Argent.h"

class Player:
	public Argent::Component::BaseActor
{
public:
	Player();
	virtual ~Player() override = default;

<<<<<<< HEAD
	void Update() override;
=======
	void Update();
	void Initialize();

private:
	int state = 0;

	GameObject* camera;

	float movement = 1.0f;

	void MoveCamera();

>>>>>>> origin/mameno-to
protected:

};

