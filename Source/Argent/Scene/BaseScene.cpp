#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	void BaseScene::Initialize()
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if(gameObject.at(i))
			{
				if(gameObject.at(i)->GetIsActive())
					gameObject.at(i)->Initialize();
			}
		}
		isInitialized = true;
	}

	void BaseScene::Finalize()
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if(gameObject.at(i))
			{
				gameObject.at(i)->Finalize();
				delete gameObject.at(i);
			}
		}
		gameObject.clear();
	}

	void BaseScene::Begin()
	{
		//todo resizeしてからのほうが処理軽いかも
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (gameObject.at(i) && gameObject.at(i)->GetIsActive())
			{
				gameObject.at(i)->Begin();
			}
		}
	}

	void BaseScene::End()
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (gameObject.at(i) && gameObject.at(i)->GetIsActive())
				gameObject.at(i)->End();
		}
	}

	void BaseScene::Update()
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (gameObject.at(i) && gameObject.at(i)->GetIsActive())
				gameObject.at(i)->Update();
		}
	}

	void BaseScene::Render()
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if(gameObject.at(i) && gameObject.at(i)->GetIsActive())
				gameObject.at(i)->Render();
		}
	}

	void BaseScene::DeleteDestroyedObject()
	{
		for(auto it = gameObject.begin(); it != gameObject.end();)
		{
			if((*it)->GetDestroyFlag())
			{
				delete (*it);
				it =gameObject.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

#ifdef _DEBUG
	void BaseScene::DrawDebug()
	{
		if (ImGui::TreeNode("Object"))
		{
			for(size_t i = 0; i < gameObject.size(); ++i)
			{
				ImGuiCheckBox(gameObject.at(i));
			}
			ImGui::TreePop();
		}
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			gameObject.at(i)->DrawDebug();
		}
	}
#endif

	void BaseScene::CloseAllDebugWindow() const
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			gameObject.at(i)->CloseAllWindow();
		}
	}

	void BaseScene::AddObject(GameObject* obj)
	{
		std::string n = ObjectNameCheck(obj->GetName(), 0);
		obj->SetName(n);
		if(isInitialized)
			obj->Initialize();
		gameObject.emplace_back(obj);
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

	//void BaseScene::DestroyGameObject(GameObject* object)
	//{
	//	//todo これアップデートとかforぶん回してる途中で読んでも大丈夫なのか？
	//	destroyedGameObject.emplace_back(object);
	//	for (auto it = gameObject.begin(); it != gameObject.end(); ++it)
	//	{
	//		if ((*it) == object)
	//		{
	//			gameObject.erase(it);
	//			break;
	//		}
	//	}
	//}
}