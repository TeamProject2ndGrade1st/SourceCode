#pragma once
#include <unordered_map>
#include <memory>

#include "BaseScene.h"

namespace Argent::Scene
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager() = default;

		void Initialize();
		void Finalize() const;
		void Begin();
		void Update();
		void End() const;
		void Render() const;
		void DrawDebug() const;
		static void SetNextScene(const std::string& s) { nextScene = s; }

		static SceneManager* Instance() { return instance;  }

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
		static SceneManager* instance;
		static std::string nextScene;
		BaseScene* currentScene;
		BaseScene* postScene;
		std::unordered_map<std::string, std::unique_ptr<BaseScene>> scenes{};
	};
}
