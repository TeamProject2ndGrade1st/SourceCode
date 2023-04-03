#pragma once
#include <string>
#include <vector>

#include "../GameObject/GameObject.h"
#include "../Component/Light.h"

namespace Argent::Scene
{
	class BaseScene
	{
	public:
		explicit BaseScene(std::string sceneName):
			sceneName(std::move(sceneName))
		{
			gameObject.clear();
			AddObject(GameObject::SceneCamera("Main Camera", true));
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
		 * \brief ���t���[����ԍŏ��ɌĂ΂��
		 */
		virtual void Begin();
		/**
		 * \brief ���t���[����ԍŌ�ɌĂ΂��
		 */
		virtual void End();
		virtual void Update();
		virtual void Render();

		void DeleteDestroyedObject();

		void DestroyGameObject(GameObject* object);

	#ifdef _DEBUG
		virtual void DrawDebug();
	#endif
		const std::string& GetName() const { return sceneName; }
		void CloseAllDebugWindow() const;
		void AddObject(GameObject* obj)
		{
			std::string n = ObjectNameCheck(obj->GetName(), 0);
			obj->SetName(n);
			gameObject.emplace_back(obj);
		}

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
		std::vector<GameObject*> destroyedGameObject{};
	};
	
}

