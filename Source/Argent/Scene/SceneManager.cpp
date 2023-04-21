#include "SceneManager.h"

//todo âΩÇ∆Ç©Ç∑ÇÈÇ±Ç∆
#include "../../Title.h"
#include "../../Game.h"
#include "../../StageSelect.h"
#include "../../Result.h"
#include "../Input/Keyboard.h"

namespace Argent::Scene
{
	SceneManager* SceneManager::instance = nullptr;
	std::string SceneManager::nextScene;
		
	SceneManager::SceneManager():
		currentScene(nullptr)
	{
		if (instance) _ASSERT_EXPR(FALSE, L"Already instantiated");
		instance = this;

		//ÉVÅ[ÉìÇÃí«â¡
		RegisterScene<Title>();
		RegisterScene<Game>();
		RegisterScene<StageSelect>();
		RegisterScene<Result>();
		/*std::unique_ptr<BaseScene> s = std::make_unique<Title>();
		scenes[s->GetName()] = std::move(s);
		
		s = std::make_unique<Game>();
		scenes[s->GetName()] = std::move(s);


		s = std::make_unique<StageSelect>();
		scenes[s->GetName()] = std::move(s);*/
	}

	void SceneManager::Initialize()
	{
		nextScene = "Title";
	}
		
	void SceneManager::Finalize() const
	{
		if(currentScene)
			currentScene->Finalize();
	}

	void SceneManager::Begin()
	{
		ChangeScene();
		if (currentScene)
			currentScene->Begin();
	}

	void SceneManager::Update()
	{
		start = end;
		end = GetTickCount();   
		
		if(currentScene)
		{
			currentScene->Update();
		}
	}

	void SceneManager::End() const
	{
		if (currentScene)
			currentScene->End();
	}

	void SceneManager::Render() const
	{
		if(currentScene)
		{
			currentScene->Render();
		}
	}

	void SceneManager::DrawDebug() const
	{
		if(currentScene)
		{
			currentScene->DrawDebug();
			double elapsedTime = (double)(end - start) / 1000;
			ImGui::InputDouble("Deltatime", &elapsedTime);
		}
	}

	void SceneManager::ChangeScene()
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
