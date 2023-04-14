#include "SceneManager.h"

//todo ‰½‚Æ‚©‚·‚é‚±‚Æ
#include "../../Title.h"
#include "../../Game.h"
#include "../Input/Keyboard.h"

namespace Argent::Scene
{
	ArSceneManager* ArSceneManager::instance = nullptr;
	std::string ArSceneManager::nextScene;
		
	ArSceneManager::ArSceneManager():
		currentScene(nullptr)
	{
		if (instance) _ASSERT_EXPR(FALSE, L"Already instantiated");
		instance = this;
		std::unique_ptr<BaseScene> s = std::make_unique<Title>("Title");
		scenes[s->GetName()] = std::move(s);
		
		s = std::make_unique<Game>("Game");
		scenes[s->GetName()] = std::move(s);
		
	}

	void ArSceneManager::Initialize()
	{
		nextScene = "Title";
	}
		
	void ArSceneManager::Finalize()
	{
		if(currentScene)
			currentScene->Finalize();
	}

	void ArSceneManager::Begin()
	{
		if (currentScene)
			currentScene->Begin();
	}

	void ArSceneManager::Update()
	{
		ChangeScene();
		if(currentScene)
		{
			currentScene->Update();
		}
	}

	void ArSceneManager::End()
	{
		if (currentScene)
			currentScene->End();
	}

	void ArSceneManager::Render()
	{
		if(currentScene)
		{
			currentScene->Render();
//#ifdef _DEBUG
			currentScene->DrawDebug();
//#endif
		}
	}

	void ArSceneManager::ChangeScene()
	{
		if(!nextScene.empty())
		{
			if(currentScene)
				currentScene->Finalize();
		
			currentScene = scenes[nextScene].get();
			nextScene.clear();
		
			if(currentScene)
				currentScene->Initialize();
		}
	}
}
