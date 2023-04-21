#include "Result.h"

void Result::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::Enter))
	{
		Argent::Scene::SceneManager::SetNextScene("Title");
	}
	BaseScene::Update();
}
