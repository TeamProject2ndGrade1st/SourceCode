#pragma once
#include "Argent/Argent.h"


class Stage:
	public Argent::Component::BaseComponent
{
public:
	Stage(const char* filePath):
		BaseComponent("Stage")
	,	filePath(filePath)
	{}
	~Stage() override = default;

	void Initialize() override;

	std::string filePath;
};

class Box:
	public Argent::Component::BaseComponent
{
public:
	Box(const char* filePath):
		BaseComponent("Box")
	,	filePath(filePath)
	{}
	~Box() override = default;

	void Initialize() override;
	const char* filePath;
};

class OwnCamp:
	public Argent::Component::BaseComponent
{
public:
	OwnCamp(const char* filePath):
		BaseComponent("OwnCamp")
	,	filePath(filePath)
	{}

	void Initialize() override;
	const char* filePath;
};

class Core:
	public Argent::Component::BaseComponent
{
public:
	Core(const char* filePath):
		BaseComponent("Core")
	,	filePath(filePath)
	{}

	void Initialize() override;
	void SubHp(float f) { hp -= f; }
	void HealHp(float f) { hp += f; }

	void Update() override;
private:
	int hp;
	const char* filePath;
};

