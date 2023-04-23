#pragma once
#include "Argent/Argent.h"
#include "BaseGun.h"

class Player:
	public Argent::Component::BaseActor
{
public:
	Player();
	virtual ~Player() override = default;

	void Initialize() override;
	void Update() override;


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
	bool useCameraControl = true;
	float offsetLength;
};

class Tmp : public Argent::Component::BaseActor
{
	void Initialize() override;
	void Finalize() override;
	void Update() override;
};
