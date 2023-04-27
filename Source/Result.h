#pragma once
#include "Argent/Argent.h"


class Result:
	public Argent::Scene::BaseScene
{
public:
	Result():
		BaseScene("Result")
	{};

	void Initialize() override;
	void Update() override;
	~Result() override = default;

};

