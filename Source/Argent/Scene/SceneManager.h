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

		void Initialize();
		void Finalize() const;
		void Begin();
		void Update();
		void End() const;
		void Render() const;
		void DrawDebug() const;
		static void SetNextScene(const std::string& s) { nextScene = s; }

		static ArSceneManager* Instance() { return instance;  }

		BaseScene* GetCurrentScene() const { return currentScene;  }

		void DeleteDestroyedObject() const
		{
			if (currentScene)
				currentScene->DeleteDestroyedObject();
		}

		template<class T>
		void RegisterScene()
		{
			std::unique_ptr<T> t = std::make_unique<T>();
			scenes[t->GetName()] = std::move(t);
		}
	private:
		void ChangeScene();

		
		
	private:
		static ArSceneManager* instance;
		static std::string nextScene;
		BaseScene* currentScene;
		std::unordered_map<std::string, std::unique_ptr<BaseScene>> scenes{};
		DWORD end;
		DWORD start;
	};
}
