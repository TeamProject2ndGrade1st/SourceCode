#include "StageSelect.h"

void StageSelect::Initialize()
{
	ClearGameObject();
	AddObject(new GameObject("StageSelectSprite", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/StageSelect.png")));
	BaseScene::Initialize();
}

void StageSelect::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter) || Argent::Input::GetKeyUp(KeyCode::Z) || Argent::Input::GetKeyUp(KeyCode::Space))
	{
		Argent::Scene::SceneManager::SetNextScene("Game");
	}
	BaseScene::Update();
}
