#pragma once
#include "Argent/Argent.h"


class Stage:
	public Argent::Component::BaseComponent
{
public:
	Stage();
	~Stage() override = default;

	void Initialize() override;
};
