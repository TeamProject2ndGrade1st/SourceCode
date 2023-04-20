#pragma once
#include "Argent/Scene/BaseScene.h"


class StageSelect:
	public Argent::Scene::BaseScene
{
public:
	StageSelect():
		BaseScene("StageSelect")
	{}


	~StageSelect()override = default;
};

