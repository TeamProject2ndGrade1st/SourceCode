#include "GameObject.h"
#include <sstream>
#include "../Graphic/Graphics.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Input/Keyboard.h"

GameObject::GameObject(std::string name, Argent::Component::BaseComponent* c) :
	isSelected(false)
	, name(std::move(name))
,	isInitialized(false)
//,	isActive(true)
{
	
	transform = new Transform();
	AddComponent(transform);
	if (c)
	{
		AddComponent(c);
	}
}

GameObject::GameObject(std::string name, std::vector<Argent::Component::BaseComponent*> com) :
	isSelected(false)
	, name(std::move(name))
	, isInitialized(false)
//,	isActive(true)
{
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

GameObject::GameObject(std::initializer_list<Argent::Component::BaseComponent*> components, std::string name) :
	isSelected(false)
	, name(name)
	, isInitialized(false)
//,	isActive(true)
{
	transform = new Transform();
	AddComponent(transform);
	for (auto&& com : components)
	{
		AddComponent(com);
	}
}

void GameObject::Initialize()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->Initialize();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->Initialize();
	}
	isInitialized = true;
}

void GameObject::Finalize()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->Finalize();
		delete components.at(i);
	}
	components.clear();
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->Finalize();
		delete childObjects.at(i);
	}
	childObjects.clear();
}

void GameObject::Begin()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->Begin();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->Begin();
	}
}

void GameObject::End()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->End();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->End();
	}
}

void GameObject::Update()
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->Update();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->Update();
	}
}

void GameObject::Render() const
{
	for(size_t i = 0; i < components.size(); ++i)
	{
		components.at(i)->Render();
	}
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->Render();
	}
}

void GameObject::DrawDebug() 
{
#ifdef _DEBUG
	if(isSelected)
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
					SetIsSelected(false);
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
				parent->SetIsSelected(true);
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
			
			components.at(i)->DrawDebug();
		}

		//AddComponent
		{
		//static bool addComponent;
		//if(ImGui::Button("Add CubeComponent", ImVec2(200, 30)))
		//{
		//	addComponent = true;
		//	//AddComponent(new Cube);
		//}

		//if(addComponent)
		//{
		//	ImGui::SetNextWindowPos(ImVec2( ImGui::GetWindowPos().x,
		//		ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2.0f), ImGuiCond_::ImGuiCond_Once);
		//	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_::ImGuiCond_Once);
		//	ImGui::Begin("Component");

		//	if(ImGui::Button("Cube"))
		//	{
		//		AddComponent(new Cube);
		//		addComponent = false;
		//	}
		//	if(ImGui::Button("Sphere"))
		//	{
		//		AddComponent(new Sphere);
		//		addComponent = false;
		//	}

		//	ImGui::End();
		//}

		}
		ImGui::End();
	}
	
	for(size_t i = 0; i < childObjects.size(); ++i)
	{
		childObjects.at(i)->DrawDebug();
	}

#endif
	
}

void GameObject::AddComponent(Argent::Component::BaseComponent* com)
{
	com->SetOwner(this);
	if (isInitialized)
		com->Initialize();
	//addComponents.emplace_back(com);
	components.emplace_back(com);
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
	childObjects.emplace_back(obj);
}

void GameObject::CloseAllWindow()
{
	CloseWindow();
	for (const auto& child : childObjects)
	{
		child->CloseAllWindow();
	}
}

void GameObject::DestroyGameObject(GameObject* object)
{
	object->willDestroy = true;
	//Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->DestroyGameObject(object);
}

GameObject* GameObject::Instantiate(const char* name, Argent::Component::BaseComponent* com)
{
	GameObject* ret = new GameObject(name, com);

	Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->AddObject(ret);
	return ret;
}

GameObject* GameObject::FindGameObject(const char* name)
{
	return Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->GetGameObject(name);
}

//if(isOpenNewDebugWindow)

	//if (ImGui::BeginMenuBar()) {
 //       if (ImGui::BeginMenu("File"))
 //       {
 //           if (ImGui::MenuItem("Save")) {

 //           }
 //           if (ImGui::MenuItem("Load")) {

 //           }

 //           ImGui::EndMenu();
 //       }
 //       ImGui::EndMenuBar();
	//}

	//ImGui::BeginChild(ImGui::GetID(static_cast<void*>(0)), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	//ImGui::Text("Child");
 //   ImGui::EndChild();

	//if(!isOpenNewDebugWindow)
	//{
	//	if(ImGui::TreeNode(name.c_str()))
	//	{
	//		for(const auto& com : components)
	//		{
	//			com->DrawDebug();
	//		}

	//		if(!childObjects.empty())
	//		{
	//			if(ImGui::TreeNode("ChildObject"))
	//			{
	//				for(const auto& obj : childObjects)
	//				{
	//					obj->DrawDebug();
	//				}
	//				ImGui::TreePop();
	//			}
	//		}
	//		ImGui::TreePop();
	//	}
	//}

