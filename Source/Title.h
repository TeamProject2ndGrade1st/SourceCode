#pragma once
#include "Argent/Scene/BaseScene.h"

class Title final :
	public Argent::Scene::BaseScene
{
public:
    Title():
		Argent::Scene::BaseScene("Title")
	{}
	~Title() override = default;
		
	void Initialize() override;
	void Begin() override;
	void Finalize() override;
	void Update() override;
	void Render() override;
#ifdef _DEBUG
	void DrawDebug() override;
#endif
private:
};

