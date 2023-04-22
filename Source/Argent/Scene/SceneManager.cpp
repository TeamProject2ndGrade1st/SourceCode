#include "SceneManager.h"

//todo 何とかすること
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
	,	postScene(nullptr)
	{
		if (instance) _ASSERT_EXPR(FALSE, L"Already instantiated");
		instance = this;

		//シーンの追加
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
		if(postScene)
		{
			static int count = 0;
			//todo 
			if(count > 3)
			{
				//todo シーン遷移するときにGPUにリソースがある状態で消し飛ばすと不正アクセスエラーを吐くので
				//もっといい方法を考える
				postScene->Finalize();
				postScene = nullptr;
				count = 0;
			}
			++count;
		}

		if (currentScene)
			currentScene->Begin();
	}

	void SceneManager::Update()
	{
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
		}
	}

	void SceneManager::ChangeScene()
	{
		if(!nextScene.empty())
		{
			if(currentScene)
			{
				//currentScene->Finalize();
				postScene = currentScene;
			}
		
			currentScene = scenes[nextScene].get();
			nextScene.clear();
		
			if(currentScene)
				currentScene->Initialize();
		}
	}
}
