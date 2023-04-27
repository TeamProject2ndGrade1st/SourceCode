#pragma once
#include "Argent/Argent.h"

class GunRenderer:
	public Argent::Component::BaseComponent
{
public:
	GunRenderer(const char* filePath):
		BaseComponent("GunRenderer")
	,	filePath(filePath)
	{}

	void Initialize() override;
private:
	const char* filePath;
};

