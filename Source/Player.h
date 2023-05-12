#pragma once
#include "Argent/Argent.h"
#include "BaseGun.h"
#include "Character.h"
#include "FriendCreater.h"

class Player: public Argent::Component::BaseActor
{
public:
	Player();
	virtual ~Player() override = default;

	void Initialize() override;
	void Begin() override;
	void Update() override;

	void DrawDebug() override;

	void Turn();

	void UpdateVerticalMove();

	void Jump(float power);
	void Fly(float power);
	void FlyUpdate();


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
	BaseGun* gun;
	std::vector<Camera*> cameraArray;
	//GameObject* gun;

	DirectX::XMFLOAT3 gunOffset;
	
	float groundPosY = 22;
	float maxPosY = 250;
	bool isGround{ true };

	DirectX::XMFLOAT3 velocity;
	float gravity{ 255.0f };
	float frictoin{50};
	float airControl{0.3f};

	float jumpPower{ 125 };

	float flyPower{ 600 };
	float flyEnergy{ 1 };
	float maxFlyEnergy{ 1 };

	FriendCreater* friendCreater{ nullptr };
};
