#include "SceneManager.h"

//todo âΩÇ∆Ç©Ç∑ÇÈÇ±Ç∆
#include "../../Title.h"
#include "../../Game.h"
#include "../../StageSelect.h"
#include "../../Result.h"
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

	void ArSceneManager::Initialize()
	{
		nextScene = "Title";
	}
		
	void ArSceneManager::Finalize() const
	{
		if(currentScene)
			currentScene->Finalize();
	}

	void ArSceneManager::Begin()
	{
		ChangeScene();
		if (currentScene)
			currentScene->Begin();
	}

	void ArSceneManager::Update()
	{
		start = end;
		end = GetTickCount();   
		
		if(currentScene)
		{
			currentScene->Update();
		}
	}

	void ArSceneManager::End() const
	{
		if (currentScene)
			currentScene->End();
	}

	void ArSceneManager::Render() const
	{
		if(currentScene)
		{
			currentScene->Render();
		}
	}

	void ArSceneManager::DrawDebug() const
	{
		if(currentScene)
		{
			currentScene->DrawDebug();
			double elapsedTime = (double)(end - start) / 1000;
			ImGui::InputDouble("Deltatime", &elapsedTime);
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
