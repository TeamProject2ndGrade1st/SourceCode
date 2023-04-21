#pragma once
#include "Argent/Argent.h"

class StageSelect:
	public Argent::Scene::BaseScene
{
public:
	StageSelect():
		BaseScene("StageSelect")
	{}

	void Update() override;

	~StageSelect()override = default;
};

