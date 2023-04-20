#include "Result.h"

void Result::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter))
	{
		Argent::Scene::ArSceneManager::SetNextScene("Title");
	}
	BaseScene::Update();
}
