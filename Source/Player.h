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


	void DrawDebug() override;


private:
	int state = 0;

	GameObject* camera;

	float movement = 1.0f;

	float sensitivity = 1.0f;

	DirectX::XMFLOAT2 mousePos;

	void MoveCamera();

	Argent::Component::Collision::RayCast* ray;
protected:
	DWORD start;
	DWORD end;
	double deltaTime;
};

