#include "BaseScene.h"
#include <algorithm>
#include "../Graphic/Graphics.h"
#include "../Component/Camera.h"
#include "../Component/Light.h"

namespace Argent::Scene
{
	void BaseScene::ClearGameObject()
	{
		objects.clear();
		objects.resize(100);
		for(auto& g : objects)
		{
			g = nullptr;
		}
	}

	void BaseScene::Initialize()
	{
		auto camera = new Camera(true, Argent::Graphics::Graphics::Instance()->GetWidth(), Argent::Graphics::Graphics::Instance()->GetHeight());
		camera->SetFov(90.0f);
		const auto c = new GameObject("Camera", camera);

		c->ReplaceTag(GameObject::Tag::MainCamera);
		AddObject(c);
		auto l = new GameObject("Light", new Light(lightIndex));
		l->ReplaceTag(GameObject::Tag::Light);
		AddObject(l);
		++lightIndex;
		l = new GameObject("Light", new Light(lightIndex));
		l->ReplaceTag(GameObject::Tag::Light);
		AddObject(l);
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if(objects.at(i))
			{
				if(objects.at(i)->GetIsActive())
					objects.at(i)->Initialize();
			}
		}
		isInitialized = true;
	}

	void BaseScene::Finalize()
	{
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if(objects.at(i))
			{
				objects.at(i)->Finalize();
				//delete objects.at(i);
			}
		}
		objects.clear();
		isInitialized = false;
		lightIndex = 0;
	}

	void BaseScene::Begin()
	{
		//todo resizeしてからのほうが処理軽いかも
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (objects.at(i) && objects.at(i)->GetIsActive())
			{
				objects.at(i)->Begin();
			}
		}
	}

	void BaseScene::End()
	{
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (objects.at(i) && objects.at(i)->GetIsActive())
				objects.at(i)->End();
		}
	}

	void BaseScene::Update()
	{
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (objects.at(i) && objects.at(i)->GetIsActive())
				objects.at(i)->EarlyUpdate();
		}

		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (objects.at(i) && objects.at(i)->GetIsActive())
				objects.at(i)->Update();
		}

		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (objects.at(i) && objects.at(i)->GetIsActive())
				objects.at(i)->LateUpdate();
		}
	}

	void BaseScene::Render()
	{
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if(objects.at(i) && objects.at(i)->GetIsActive())
				objects.at(i)->Render();
		}
	}

	void BaseScene::DeleteDestroyedObject()
	{
		for(auto it = objects.begin(); it != objects.end(); ++it)
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
			for(size_t i = 0; i < objects.size(); ++i)
			{
				if(!objects.at(i).get()) continue;
					ImGuiCheckBox(objects.at(i).get());
			}
			ImGui::TreePop();
		}
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (!objects.at(i)) continue;
				objects.at(i)->DrawDebug();
		}
	}

	void BaseScene::DrawDebugNumGameObject() const
	{
		int numGameObject{};
		for(auto& g : objects)
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
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if (!objects.at(i)) continue;
			objects.at(i)->CloseAllWindow();
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
			const UINT size = objects.size();
			objects.resize(size + 100);
			objects.at(size).reset(obj);
		}
		else
		{
			objects.at(i).reset(obj);
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
		for(size_t i = 0; i < objects.size(); ++i)
		{
			if(!objects.at(i))
			{
				return i;
			}
		}
		return -1;
	}
 
}