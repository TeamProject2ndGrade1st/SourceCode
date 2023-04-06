#pragma once
#include "Argent/Argent.h"


class BaseGun:
	public Argent::Component::BaseActor
{
public:
	BaseGun();
	virtual ~BaseGun() override = default;

	virtual void Shot();
	void Initialize() override;
	void Update() override;
	//virtual void Recoil() = 0;
protected:
	
};

