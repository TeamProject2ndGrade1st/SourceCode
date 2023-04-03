#pragma once
#include "Argent/Scene/BaseScene.h"

class Title final :
	public Argent::Scene::BaseScene
{
public:
    Title(std::string sceneName):
		Argent::Scene::BaseScene(sceneName)
	{}
	~Title() override = default;
		
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Render() override;
#ifdef _DEBUG
	void DrawDebug() override;
#endif
private:
};

