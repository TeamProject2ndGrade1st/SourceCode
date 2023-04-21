#include "StageSelect.h"

void StageSelect::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter))
	{
		Argent::Scene::SceneManager::SetNextScene("Game");
	}
	BaseScene::Update();
}