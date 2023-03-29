#pragma once
#include "Argent/Component/ArComponent.h"


class MrIncredible:
	public Argent::Component::ArComponent
{
public:
	MrIncredible();
	~MrIncredible() override = default;


	void Initialize() override;
};

