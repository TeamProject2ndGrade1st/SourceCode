#include "Game.h"
#include "Argent/Argent.h"

void Game::Initialize()
{
	BaseScene::Initialize();
}

void Game::Finalize()
{
	BaseScene::Finalize();
}

void Game::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter))
	{
		Argent::Scene::SceneManager::SetNextScene("Result");
	}
	BaseScene::Update();
}

void Game::Render()
{
	BaseScene::Render();
}
