#include "Title.h"
#include "Argent/Argent.h"

void Title::Initialize()
{
	ClearGameObject();

	AddObject(new GameObject("TitleSprite", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Title.png")));

	
	BaseScene::Initialize();
}

void Title::Begin()
{
	BaseScene::Begin();
}

void Title::Finalize()
{
	BaseScene::Finalize();
}

void Title::Update()
{
	BaseScene::Update();

	if(Argent::Input::GetKeyUp(KeyCode::F2))
	{
		Argent::Scene::SceneManager::SetNextScene("StageSelect");
	}
}

void Title::Render()
{
	BaseScene::Render();
}

#ifdef _DEBUG
void Title::DrawDebug()
{
	BaseScene::DrawDebug();
}
#endif

