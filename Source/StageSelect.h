#pragma once
#include "Argent/Argent.h"

class StageSelect:
	public Argent::Scene::BaseScene
{
public:
	StageSelect():
		BaseScene("StageSelect")
	{}

	void Initialize() override;
	void Update() override;

	~StageSelect()override = default;
};

