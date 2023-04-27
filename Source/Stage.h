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
	void SubHp(int f) { hp -= f; }
	void HealHp(int f) { hp += f; }

	void Update() override;
private:
	int hp;
	const char* filePath;
};

class TutorialStage:
	public Argent::Component::BaseComponent
{
public:
	TutorialStage(const char* filePath):
		BaseComponent("TutorialStage")
	,	filePath(filePath)
	{}

	~TutorialStage() override = default;


	void Initialize() override;

private:
	const char* filePath;
};

class Barricade:
	public Argent::Component::BaseComponent
{
public:
	Barricade(const char* filePath):
		BaseComponent("Barricade")
	,	filePath(filePath)
	{}


	void Initialize() override;

private:
	const char* filePath;
};

class Honeycomb:
	public Argent::Component::BaseComponent
{
public:
	Honeycomb(const char* filePath):
		BaseComponent("HoneyComb")
	,	filePath(filePath)
	{}

	void Initialize() override;

private:
	const char* filePath;
};