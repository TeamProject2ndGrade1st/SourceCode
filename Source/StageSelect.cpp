#include "StageSelect.h"

void StageSelect::Initialize()
{
	ClearGameObject();
	AddObject(new GameObject("StageSelectSprite", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/StageSelect.png")));
	BaseScene::Initialize();
}

void StageSelect::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::F2))
	{
		Argent::Scene::SceneManager::SetNextScene("Game");
	}
	BaseScene::Update();
}
