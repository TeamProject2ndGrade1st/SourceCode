#include "GameObject.h"
#include <sstream>
#include "../Graphic/Graphics.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Input/Keyboard.h"

GameObject::GameObject(std::string name, Argent::Component::BaseComponent* c) :
	isDrawDebug(false)
	, name(std::move(name))
,	isInitialized(false)
,	isActive(true)
{
	childObjects.clear();
	childObjects.resize(10);
	for(auto& cObj : childObjects)
	{
		cObj.reset(nullptr);
	}
	components.clear();
	components.resize(10);
	for(auto& com : components)
	{
		com.reset(nullptr);
	}
	transform = new Transform();
	AddComponent(transform);
	if (c)
	{
		AddComponent(c);
	}
	
}

GameObject::GameObject(std::string name, std::vector<Argent::Component::BaseComponent*> com) :
	isDrawDebug(false)
	, name(std::move(name))
	, isInitialized(false)
,	isActive(true)
{
	childObjects.clear();
	childObjects.resize(10);
	for(auto& cObj : childObjects)
	{
		cObj.reset(nullptr);
	}
	components.clear();
	components.resize(10);
	for(auto& com : components)
	{
		com.reset(nullptr);
	}
	transform = new Transform();
	AddComponent(transform);

	if(com.size() == 1)
	{
		AddComponent(com.at(0));
	}
	else
	{
		for (size_t i = 0; i < com.size(); ++i)
		{
			AddChild(new GameObject(std::to_string(i), com.at(i)));
		}
	}
}

GameObject::GameObject(std::initializer_list<Argent::Component::BaseComponent*> coms, std::string name) :
	isDrawDebug(false)
	, name(name)
	, isInitialized(false)
,	isActive(true)
{
	childObjects.clear();
	childObjects.resize(10);
	for(auto& cObj : childObjects)
	{
		cObj.reset(nullptr);
	}
	components.clear();
	components.resize(10);
	for(auto& com : components)
	{
		com.reset(nullptr);
	}
	transform = new Transform();
	AddComponent(transform);
	for (auto&& com : coms)
	{
		AddComponent(com);
	}
}

void GameObject::Initialize()
{
	isInitialized = true;
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->Initialize();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->Initialize();
	}
}

void GameObject::Finalize()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->Finalize();
	}
	components.clear();
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->Finalize();
	}
	childObjects.clear();
}

void GameObject::Begin()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->Begin();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->Begin();
	}
}

void GameObject::EarlyUpdate()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->EarlyUpdate();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->EarlyUpdate();
	}
}

void GameObject::End()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->End();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->End();
	}
}

void GameObject::Update()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->Update();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->Update();
	}
}

void GameObject::LateUpdate()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->LateUpdate();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->LateUpdate();
	}
}

void GameObject::Render() const
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(components.at(i))
			components.at(i)->Render();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->Render();
	}
}

void GameObject::DrawDebug() 
{

	if(isDrawDebug)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2( 900, 50), ImGuiCond_::ImGuiCond_Once);
		ImGui::Begin(name.c_str(),nullptr, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
		    if (ImGui::BeginMenu("Menu"))
		    {
		        if (ImGui::MenuItem("Reset"))
				{
					Initialize();
		        }

				if(ImGui::MenuItem("Close"))
				{
					SetIsDrawDebug(false);
				}
		        ImGui::EndMenu();
		    }
		    ImGui::EndMenuBar();
		}
		if(parent)
		{
			std::ostringstream oss;
			oss << "Parent Name : " << parent-> GetName();
			//ImGui::BulletText(oss.str().c_str());
			if(ImGui::Button(oss.str().c_str()))
			{
				parent->SetIsDrawDebug(true);
				CloseWindow();
			}
		}
		if(ImGui::Checkbox("Active", &isActive))
		{
			if(!isInitialized)
				Initialize();
		}

		for(size_t i = 0; i < components.size(); ++i)
		{
			if(components.at(i))
				components.at(i)->DrawDebug();	
		}
		ImGui::End();
	}
	
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->DrawDebug();
	}
}

void GameObject::AddComponent(Argent::Component::BaseComponent* com)
{
	com->SetOwner(this);
	std::string n = ComponentNameCheck(com->GetName());
	com->SetName(n.c_str());

	if (isInitialized)
		com->Initialize();

	const int64_t index = FindNullComponentIndex();
	if(index < 0)
	{
		size_t size = components.size(); 
		components.resize(size + 10);
		for(size_t i = size; i < components.size(); ++i)
		{
			components.at(i).reset(nullptr);
		}
		components.at(size).reset(com);
	}
	else
	{
		components.at(index).reset(com);
	}
}

void GameObject::AddComponent(std::vector<Argent::Component::BaseComponent*> com)
{
	if(com.size() == 1)
	{
		AddComponent(com.at(0));
	}
	else
	{
		for (size_t i = 0; i < com.size(); ++i)
		{
			AddChild(new GameObject(std::to_string(i), com.at(i)));
		}
	}
}

void GameObject::AddChild(GameObject* obj)
{
	obj->SetParent(this);
	const int64_t index = FindNullChildIndex();
	if(index < 0)
	{
		const size_t size = childObjects.size();
		childObjects.resize(size + 10);
		childObjects.at(size).reset(obj);
	}
	else
	{
		childObjects.at(index).reset(obj);
	}
}

void GameObject::CloseAllWindow()
{
	CloseWindow();
	for (size_t i = 0; i < childObjects.size(); ++i)
	{
		if(childObjects.at(i))
			childObjects.at(i)->CloseAllWindow();
	}
}

void GameObject::Destroy(GameObject* object)
{
	if(!object) return;
	object->willDestroy = true;
	object->SetActive(false);
	//すべての子オブジェクトもdestroy関数に入れる
	for(auto it = object->begin(); it != object->end(); ++it)
	{
		Destroy((*it).get());
	}
}

GameObject* GameObject::Instantiate(const char* name, Argent::Component::BaseComponent* com)
{
	GameObject* ret = new GameObject(name, com);

	Argent::Scene::SceneManager::Instance()->GetCurrentScene()->AddObject(ret);
	return ret;
}

GameObject* GameObject::Instantiate(const char* name, std::vector<Argent::Component::BaseComponent*> com)
{
	GameObject* ret = new GameObject(name, com);
	Argent::Scene::SceneManager::Instance()->GetCurrentScene()->AddObject(ret);
	return ret;
}

GameObject* GameObject::FindByName(const char* name)
{
	return Argent::Scene::SceneManager::Instance()->GetCurrentScene()->GetGameObject(name);
}

bool GameObject::FindByTag(Tag tag, std::vector<GameObject*>& objArray)
{
	const auto t = static_cast<unsigned>(tag);
	const auto s = Argent::Scene::SceneManager::Instance()->GetCurrentScene();
	for(auto it = s->begin(); it != s->end(); ++it)
	{
		if(static_cast<unsigned>((*it)->GetTag()) & t)
		{
			objArray.emplace_back((*it).get());
		}
	}

	return objArray.size() == 0;
}

int64_t GameObject::FindNullChildIndex() const
{
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		if(!childObjects.at(i)) return i;
	}
	return -1;
}

int64_t GameObject::FindNullComponentIndex() const
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		if(!components.at(i)) return i;
	}
	return -1;
}
