#pragma once
#include <string>
#include <vector>

#include "../GameObject/GameObject.h"
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
			objects.clear();
			objects.resize(100);
		}
		virtual ~BaseScene() = default;
		BaseScene(const BaseScene&) = delete;
		BaseScene(const BaseScene&&) = delete;
		BaseScene& operator=(const BaseScene&) = delete;
		BaseScene& operator=(const BaseScene&&) = delete;


		void ClearGameObject();
		virtual void Initialize();
		virtual void Finalize();
		/**
		 * \brief 毎フレーム一番最初に呼ばれる
		 */
		virtual void Begin();
		virtual void Update();
		virtual void Render();

		/**
		 * \brief 毎フレーム一番最後に呼ばれる
		 */
		virtual void End();

		virtual void DrawDebug();

		void DrawDebugNumGameObject() const;

		void DeleteDestroyedObject();

		std::vector<std::unique_ptr<GameObject>>::iterator begin() { return objects.begin(); }
		std::vector<std::unique_ptr<GameObject>>::iterator end() { return objects.end(); }

		const std::string& GetName() const { return sceneName; }
		void CloseAllDebugWindow() const;
		void AddObject(GameObject* obj);
		
		std::string ObjectNameCheck(std::string name, int num = 0, bool isChecked = false)
		{
			for(const auto& object : objects)
			{
				if (!object) continue;
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
			for(const auto& obj : objects)
			{
				if(obj->GetName() == objectName)
				{
					return obj.get();
				}
			}
			return nullptr;
		}

		int64_t FindNullObjectIndex() const;
	protected:
		const std::string sceneName;
	private:
		std::vector<std::unique_ptr<GameObject>> objects{};
		bool isInitialized;
		int lightIndex = 0;
	private:
	};
	
}

