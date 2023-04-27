#include "SceneManager.h"

//todo ���Ƃ����邱��
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

		//�V�[���̒ǉ�
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
				//todo �V�[���J�ڂ���Ƃ���GPU�Ƀ��\�[�X�������Ԃŏ�����΂��ƕs���A�N�Z�X�G���[��f���̂�
				//�����Ƃ������@���l����
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
