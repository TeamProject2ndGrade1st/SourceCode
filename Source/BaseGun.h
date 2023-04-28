#pragma once
#include "Argent/Argent.h"
#include "Bullet.h"

class BaseGun:
	public Argent::Component::BaseComponent
{
public:
	BaseGun(const char* name);
	virtual ~BaseGun() override = default;

	void Initialize() override;
	void Update() final;
	void Finalize() override;

	void DrawDebug() override;

	float fireRate;
	int damage;
	float speed;

	DirectX::XMFLOAT3 offset;

	Bullet::Mode mode;
	GameObject* lmg;
#ifdef _DEBUG
	bool enableShot = true;

#endif
};