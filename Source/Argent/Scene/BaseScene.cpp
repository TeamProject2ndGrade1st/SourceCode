#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	void BaseScene::ClearGameObject()
	{
		gameObject.clear();
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
		//todo resizeÇµÇƒÇ©ÇÁÇÃÇŸÇ§Ç™èàóùåyÇ¢Ç©Ç‡
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
		for(auto it = gameObject.begin(); it != gameObject.end();)
		{
			if((*it)->GetDestroyFlag())
			{
				(*it)->Finalize();
				delete (*it);
				it =gameObject.erase(it);
			}
			else
			{
				++it;
			}
		}
	}


	void BaseScene::DrawDebug()
	{
		ImGui::Text(GetName().c_str());
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
	}

	// 
}