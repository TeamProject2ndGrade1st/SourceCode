#include "StageSelect.h"

void StageSelect::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter))
	{
		Argent::Scene::ArSceneManager::SetNextScene("Game");
	}
	BaseScene::Update();
}