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
private:
	int state = 0;

	GameObject* camera;

	float movement = 1.0f;

	void MoveCamera();
protected:

};

