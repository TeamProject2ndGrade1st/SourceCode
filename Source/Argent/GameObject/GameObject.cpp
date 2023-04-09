#include "GameObject.h"
#include <sstream>
#include "../Graphic/Graphics.h"
#include "../Component/Camera.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneManager.h"


GameObject::GameObject(std::string name, Argent::Component::BaseComponent* c) :
	isSelected(false)
	, name(std::move(name))
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
{
	transform = new Transform();
	AddComponent(transform);
	for (auto&& com : components)
	{
		AddComponent(com);
	}
}


void GameObject::AddComponent(Argent::Component::BaseComponent* com)
{
	com->SetOwner(this);
	components.emplace_back(com);
}

void GameObject::AddComponent(std::vector<Argent::Component::BaseComponent*> com)
{
	for (size_t i = 0; i < com.size(); ++i)
	{
		if(com.size() == 1)
		{
			AddComponent(com.at(i));
		}
		else
			AddChild(new GameObject(std::to_string(i), com.at(i)));
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

GameObject* GameObject::SceneCamera(const std::string& name, bool setSceneCamera)
{
	return new GameObject({
			new Camera(setSceneCamera, Argent::Graphics::ArGraphics::Instance()->GetWidth(), Argent::Graphics::ArGraphics::Instance()->GetHeight()),
			new CameraController
		},
		name);
}

void GameObject::DestroyGameObject(GameObject* object)
{
	Argent::Scene::ArSceneManager::Instance()->GetCurrentScene()->DestroyGameObject(object);
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

void GameObject::Initialize()
{
	for(const auto& com : components)
	{
		com->Initialize();
	}
	for(const auto& obj : childObjects)
	{
		obj->Initialize();
	}
}

void GameObject::Finalize()
{
	for(const auto& com : components)
	{
		com->Finalize();
		delete com;
	}
	components.clear();
	for(const auto& obj : childObjects)
	{
		obj->Finalize();
		delete obj;
	}
	childObjects.clear();
}

void GameObject::Begin()
{
	for (const auto& com : components)
	{
		com->End();
	}
	for (const auto& obj : childObjects)
	{
		obj->Begin();
	}
}

void GameObject::End()
{
	for (const auto& com : components)
	{
		com->End();
	}
	for (const auto& obj : childObjects)
	{
		obj->Begin();
	}
}

void GameObject::Update()
{
	for(const auto& com : components)
	{
		com->Update();
	}
	for(const auto& obj : childObjects)
	{
		obj->Update();
	}
}

void GameObject::Render() const
{
	for(const auto& com : components)
	{
		com->Render();
	}
	for(const auto& obj : childObjects)
	{
		obj->Render();
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
		for(const auto& com : components)
		{
			com->DrawDebug();
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
	
	for(const auto& obj : childObjects)
	{
		obj->DrawDebug();
	}

#endif
	
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

