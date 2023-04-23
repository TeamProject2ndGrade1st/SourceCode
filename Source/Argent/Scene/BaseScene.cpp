#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	void BaseScene::ClearGameObject()
	{
		gameObject.clear();
		gameObject.resize(100);
		for(auto& g : gameObject)
		{
			g = nullptr;
		}
	}

	void BaseScene::Initialize()
	{
		const auto c = new GameObject("Camera", new Camera(true, Argent::Graphics::Graphics::Instance()->GetWidth(), Argent::Graphics::Graphics::Instance()->GetHeight()));
		c->SetTag(GameObject::Tag::MainCamera);
		AddObject(c);
		AddObject(new GameObject("Light", new Light));
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
		isInitialized = false;
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
				gameObject.at(i)->EarlyUpdate();
		}

		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (gameObject.at(i) && gameObject.at(i)->GetIsActive())
				gameObject.at(i)->Update();
		}

		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (gameObject.at(i) && gameObject.at(i)->GetIsActive())
				gameObject.at(i)->LateUpdate();
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
		for(auto it = gameObject.begin(); it != gameObject.end(); ++it)
		{
			if (!(*it)) continue;
			

			if((*it)->GetDestroyFlag())
			{
				(*it)->Finalize();
				delete (*it);
				(*it) = nullptr;
			}
		}
	}

	void BaseScene::DrawDebug()
	{
		ImGui::Text(GetName().c_str());
		int numGameObject{};
		for(auto& g : gameObject)
		{
			if(g)
			{
				++numGameObject;
			}
		}
		ImGui::InputInt("Num GameObject", &numGameObject);
		if (ImGui::TreeNode("Object"))
		{
			for(size_t i = 0; i < gameObject.size(); ++i)
			{
				if(!gameObject.at(i)) continue;
				ImGuiCheckBox(gameObject.at(i));
			}
			ImGui::TreePop();
		}
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (!gameObject.at(i)) continue;
			gameObject.at(i)->DrawDebug();
		}
	}


	void BaseScene::CloseAllDebugWindow() const
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (!gameObject.at(i)) continue;
			gameObject.at(i)->CloseAllWindow();
		}
	}

	void BaseScene::AddObject(GameObject* obj)
	{
		std::string n = ObjectNameCheck(obj->GetName(), 0);
		obj->SetName(n);
		if(isInitialized)
			obj->Initialize();
		int i = FindNullObjectIndex();
		if(i < 0)
		{
			const UINT size = gameObject.size();
			gameObject.resize(size + 100);
			gameObject.at(size) = obj;
		}
		else
		{
			gameObject.at(i) = obj;
		}
	}

	void BaseScene::ImGuiCheckBox(GameObject* obj)
	{
		if (ImGui::Button(obj->GetName().c_str()))
		{
			//CloseAllDebugWindow();
			obj->SetIsSelected(true);
		}

		if (obj->GetChildCount() == 0) return;
		if (ImGui::TreeNode(obj->GetName().c_str()))
		{
			for (const auto& child : *obj)
			{
				if(!child)
				ImGuiCheckBox(child);
			}
			ImGui::TreePop();
		}
	}

	UINT BaseScene::FindNullObjectIndex() const
	{
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if(!gameObject.at(i))
			{
				return i;
			}
		}
		return -1;
	}
 
}