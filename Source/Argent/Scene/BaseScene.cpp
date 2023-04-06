#include "BaseScene.h"
#include <algorithm>
#include "../Component/Camera.h"
#include "../Graphic/Graphics.h"

namespace Argent::Scene
{
	void BaseScene::Initialize()
	{
		Camera* c{};
		Light* l{};
		for (const auto& object : gameObject)
		{
			if (!object) continue;

			object->Initialize();
			if (!c)
			{
				c = object->GetComponent<Camera>();
				if (!c)
				{
					if (object->GetChild<GameObject>())
						c = object->GetChild<GameObject>()->GetComponent<Camera>();
				}
			}
			if (!l)
			{
				l = object->GetComponent<Light>();
				if (!l)
				{
					if (object->GetChild<GameObject>())
						l = object->GetChild<GameObject>()->GetComponent<Light>();
				}
			}
		}
		if(!c || !l)
		{
			for (const auto& object : addObject)
			{
				if (!object) continue;

				object->Initialize();
				if (!c)
				{
					c = object->GetComponent<Camera>();
					if (!c)
					{
						if (object->GetChild<GameObject>())
							c = object->GetChild<GameObject>()->GetComponent<Camera>();
					}
				}
				if (!l)
				{
					l = object->GetComponent<Light>();
					if (!l)
					{
						if (object->GetChild<GameObject>())
							l = object->GetChild<GameObject>()->GetComponent<Light>();
					}
				}
			}
			
		}
		Argent::Graphics::ArGraphics::Instance()->SetCamera(c);
		Argent::Graphics::ArGraphics::Instance()->SetLight(l);
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
		//todo resize���Ă���̂ق��������y������
		for(const auto& obj : addObject)
		{
			if (obj)
			{
				//todo initialize����^�C�~���O�͖{���ɂ����ł����H
				obj->Initialize();
				gameObject.emplace_back(std::move(obj));
			}
		}
		addObject.clear();
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