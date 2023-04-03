#pragma once
#include <unordered_map>
#include <memory>

#include "BaseScene.h"

namespace Argent::Scene
{
	class ArSceneManager
	{
	public:
		ArSceneManager();
		~ArSceneManager() = default;
		
		//void Execute();
		void Initialize();
		void Finalize();
		void Begin();
		void Update();
		void End();
		void Render();
		static void SetNextScene(const std::string& s) { nextScene = s; }

		static ArSceneManager* Instance() { return instance;  }

		BaseScene* GetCurrentScene() const { return currentScene;  }

		void DeleteDestroyedObject() const
		{
			if (currentScene)
				currentScene->DeleteDestroyedObject();
		}
	private:
		void ChangeScene();
		
	private:
		static ArSceneManager* instance;
		static std::string nextScene;
		BaseScene* currentScene;
		std::unordered_map<std::string, std::unique_ptr<BaseScene>> scenes;

	};
}
