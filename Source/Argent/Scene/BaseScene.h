#pragma once
#include <string>
#include <vector>

#include "../GameObject/GameObject.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	class BaseScene
	{
	public:
		explicit BaseScene(std::string sceneName):
			sceneName(std::move(sceneName))
		,	isInitialized(false)
		{
			gameObject.clear();
			AddObject(new GameObject("Camera", new Camera(true, Argent::Graphics::Graphics::Instance()->GetWidth(), Argent::Graphics::Graphics::Instance()->GetHeight())));
			AddObject(new GameObject("Light", new Light));
		}
		virtual ~BaseScene() = default;
		BaseScene(const BaseScene&) = delete;
		BaseScene(const BaseScene&&) = delete;
		BaseScene& operator=(const BaseScene&) = delete;
		BaseScene& operator=(const BaseScene&&) = delete;

		
		virtual void Initialize();
		virtual void Finalize();
		/**
		 * \brief 毎フレーム一番最初に呼ばれる
		 */
		virtual void Begin();
		/**
		 * \brief 毎フレーム一番最後に呼ばれる
		 */
		virtual void End();
		virtual void Update();
		virtual void Render();

		virtual void DrawDebug();


		void DeleteDestroyedObject();
		//void DestroyGameObject(GameObject* object);


		const std::string& GetName() const { return sceneName; }
		void CloseAllDebugWindow() const;
		void AddObject(GameObject* obj);
		

		std::string ObjectNameCheck(std::string name, int num = 0, bool isChecked = false)
		{
			for(const auto& object : gameObject)
			{
				if(object->GetName() == name)
				{
					if(isChecked)
					{
						name.erase(name.find('('));
					}
					std::string n = name + "(" + std::to_string(num) + ")";
					++num;
					name = ObjectNameCheck(n, num, true);
				}
			}
			return name;
		}

		void ImGuiCheckBox(GameObject* obj);

		GameObject* GetGameObject(const std::string& objectName) const
		{
			for(const auto& obj : gameObject)
			{
				if(obj->GetName() == objectName)
				{
					return obj;
				}
			}
			return nullptr;
		}

	protected:
		const std::string sceneName;
		std::vector<GameObject*> gameObject{};
		//std::vector<GameObject*> destroyedGameObject{};
		bool isInitialized;
	private:
	};
	
}

