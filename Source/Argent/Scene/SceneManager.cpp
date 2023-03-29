#include "SceneManager.h"

//todo 何とかすること
#include "../../Title.h"
#include "../../Game.h"
#include "../Input/Keyboard.h"

namespace Argent::SceneManagement
{
		
	std::string ArSceneManager::nextScene;
		
	ArSceneManager::ArSceneManager():
		currentScene(nullptr)
	{
		std::unique_ptr<Scene> s = std::make_unique<Title>("Title");
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

	void ArSceneManager::Update()
	{
		ChangeScene();
		if(currentScene)
		{
			currentScene->Update();
		}
	}

	void ArSceneManager::Render()
	{
		if(currentScene)
		{
			currentScene->Render();
#ifdef _DEBUG
			currentScene->DrawDebug();
#endif
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
