#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"

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
				//delete gameObject.at(i);
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
				//todo ゲームオブジェクトを破壊するまでの待ち時間をどうにかすること
				(*it)->elapsedTimeFromDestroyed += 1;
				if((*it)->elapsedTimeFromDestroyed < 3) continue;
				(*it)->Finalize();
				(*it).reset(nullptr);
				//delete (*it);
				//(*it) = nullptr;
			}
		}
	}

	void BaseScene::DrawDebug()
	{
		ImGui::Text(GetName().c_str());

		DrawDebugNumGameObject();

		if (ImGui::TreeNode("Object"))
		{
			for(size_t i = 0; i < gameObject.size(); ++i)
			{
				if(!gameObject.at(i).get()) continue;
					ImGuiCheckBox(gameObject.at(i).get());
			}
			ImGui::TreePop();
		}
		for(size_t i = 0; i < gameObject.size(); ++i)
		{
			if (!gameObject.at(i)) continue;
				gameObject.at(i)->DrawDebug();
		}
	}

	void BaseScene::DrawDebugNumGameObject() const
	{
		int numGameObject{};
		for(auto& g : gameObject)
		{
			if(g)
			{
				++numGameObject;
			}
		}
		ImGui::InputInt("Num GameObject", &numGameObject);
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
			gameObject.at(size).reset(obj);
		}
		else
		{
			gameObject.at(i).reset(obj);
		}
	}

	void BaseScene::ImGuiCheckBox(GameObject* obj)
	{
		if (ImGui::Button(obj->GetName().c_str()))
		{
			if(obj)
				obj->SetIsDrawDebug(true);
		}

		if (obj->GetChildCount() == 0) return;
		int numValidChild = 0;
		for(const auto& child : *obj)
		{
			if(child)
				++numValidChild;
		}
		if(numValidChild == 0) return;

		if (ImGui::TreeNode(obj->GetName().c_str()))
		{
			for (const auto& child : *obj)
			{
				if(child)
					ImGuiCheckBox(child.get());
			}
			ImGui::TreePop();
		}
	}

	int64_t BaseScene::FindNullObjectIndex() const
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