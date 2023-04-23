#pragma once
#include "Argent/Argent.h"
#include "Bullet.h"

class BaseGun:
	public Argent::Component::BaseComponent
{
public:
	BaseGun(const char* name);
	virtual ~BaseGun() override = default;

	
	void Update() final;


	void DrawDebug() override;

	float fireRate;
	int damage;
	float speed;

#ifdef _DEBUG
	bool enableShot = false;
#endif
};