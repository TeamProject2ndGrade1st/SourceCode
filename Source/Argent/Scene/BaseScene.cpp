#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	void BaseScene::Initialize()
	{
		AddObjectToGameObject();
	}

	void BaseScene::Finalize()
	{
		for (const auto& object : gameObject)
		{
			if (!object) continue;

			object->Finalize();
			delete object;
		}
		gameObject.clear();
	}

	void BaseScene::Begin()
	{
		//todo resizeしてからのほうが処理軽いかも
		AddObjectToGameObject();
		for (const auto& object : gameObject)
		{
			if (!object) continue;
			object->Begin();
		}
	}

	void BaseScene::End()
	{
		for (const auto& object : gameObject)
		{
			if (!object) continue;
			object->End();
		}
	}

	void BaseScene::Update()
	{
		for (const auto& object : gameObject)
		{
			if (!object) continue;
			object->Update();
		}
	}

	void BaseScene::Render()
	{
		for (const auto& object : gameObject)
		{
			if (!object) continue;
			object->Render();
		}
	}

	void BaseScene::DeleteDestroyedObject()
	{
		for(auto it = destroyedGameObject.begin(); it != destroyedGameObject.end();)
		{
			GameObject* g = (*it);
			it = destroyedGameObject.erase(it);
			g->Finalize();
			delete g;
		}
	}

#ifdef _DEBUG
	void BaseScene::DrawDebug()
	{
		if (ImGui::TreeNode("Object"))
		{
			for (const auto& object : gameObject)
			{
				ImGuiCheckBox(object);
			}
			ImGui::TreePop();
		}
		for (const auto& object : gameObject)
		{
			object->DrawDebug();
		}
	}
#endif

	void BaseScene::CloseAllDebugWindow() const
	{
		for (const auto& obj : gameObject)
		{
			obj->CloseAllWindow();
		}
	}

	void BaseScene::AddObject(GameObject* obj)
	{
		std::string n = ObjectNameCheck(obj->GetName(), 0);
		obj->SetName(n);
		addObject.emplace_back(obj);
	}

	void BaseScene::ImGuiCheckBox(GameObject* obj)
	{
#ifdef _DEBUG
		
		if (ImGui::Button(obj->GetName().c_str()))
		{
			CloseAllDebugWindow();
			obj->SetIsSelected(true);
		}

		if (obj->GetChildCount() == 0) return;
		if (ImGui::TreeNode(obj->GetName().c_str()))
		{
			for (const auto& child : *obj)
			{
				ImGuiCheckBox(child);
			}
			ImGui::TreePop();
		}
#endif
	}

	void BaseScene::AddObjectToGameObject()
	{
		for(const auto& obj : addObject)
		{
			if (obj)
			{
				//todo initializeするタイミングは本当にここでいい？
				obj->Initialize();
				gameObject.emplace_back(std::move(obj));
			}
		}
		addObject.clear();
	}

	void Argent::Scene::BaseScene::DestroyGameObject(GameObject* object)
	{
		destroyedGameObject.emplace_back(object);
		for (auto it = gameObject.begin(); it != gameObject.end(); ++it)
		{
			if ((*it) == object)
			{
				gameObject.erase(it);
				break;
			}
		}
	}
}