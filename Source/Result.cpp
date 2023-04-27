#include "Result.h"

void Result::Initialize()
{
	ClearGameObject();
	AddObject(new GameObject("ResultSprite", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Result.png")));
	BaseScene::Initialize();
}

void Result::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::F2))
	{
		Argent::Scene::SceneManager::SetNextScene("Title");
	}
	BaseScene::Update();
}
