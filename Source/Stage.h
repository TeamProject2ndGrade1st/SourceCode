#pragma once
#include "Argent/Argent.h"


class Stage:
	public Argent::Component::BaseComponent
{
public:
	Stage(const char* filePath);
	~Stage() override = default;

	void Initialize() override;

	std::string filePath;
};

