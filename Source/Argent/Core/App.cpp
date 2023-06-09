#include "App.h"
#include "../../../imgui/ImguiCtrl.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Input/Keyboard.h"
#include "../Component/ColliderManager.h"
#include "../Resource/AudioManager.h"
#include "../Input/Mouse.h"

namespace Argent::App
{

	bool ArApp::quit = false;

	ArApp::ArApp(HINSTANCE hInstance, LONG width, LONG height, const char* appName, bool isFullScreen)
	{
		static bool isInstantiated{ FALSE };
		if(isInstantiated)_ASSERT_EXPR(FALSE, L"Instance is already existed");

		window = std::make_unique<Window::ArWindow>(hInstance, width, height);
		arGfx = std::make_unique<Graphics::Graphics>(window->GetHandle());
		effectManager = std::make_unique<Argent::Resource::Effect::EffectManager>(arGfx->GetDevice(), arGfx->GetCommandQueue(), arGfx->GetNumBackBuffers());

		isInstantiated = true;
	}

	void ArApp::Initialize() const
	{
		arGfx->Initialize();
#ifdef _DEBUG
		ImguiCtrl::Initialize(window->GetHandle(), arGfx->GetDevice(), arGfx->GetGUIHeap());
		ImGui::GetStyle().Alpha = 0.7f;
#endif
		Resource::ResourceManager::Instance().Initialize();
	}

	int ArApp::Execute() const
	{
		Initialize();
			
		Scene::SceneManager arSceneManager;
		arSceneManager.Initialize();
		Argent::Resource::Audio::AudioManager::Instance().Initialize();
		while (MainLoop(window->GetHandle()))
		{
			Argent::Input::Keyboard::Instance().Update();
			Argent::Input::Mouse::Instance().Update();

#ifdef _DEBUG
			ImguiCtrl::Begin("Main Window");
#endif
			arSceneManager.Begin();

			arSceneManager.Update();
			effectManager->Update();
			Argent::Resource::Audio::AudioManager::Instance().Update();
#ifdef _DEBUG
			Argent::Resource::Audio::AudioManager::Instance().DrawDebug();
#endif
			Argent::Collider::ColliderManager::Instance().CollisionDetection();

			arSceneManager.DeleteDestroyedObject();

			Resource::ResourceManager::Instance().DrawDebug();
			arGfx->Begin();
			arSceneManager.Render();
			effectManager->Render();

#ifdef _DEBUG
			arSceneManager.DrawDebug();
#endif
			arSceneManager.End();
			

			arGfx->End();

			if (quit) break;
		}
		Resource::ResourceManager::Instance().Serialize();

		arGfx->Terminate();
		arSceneManager.Finalize();
#ifdef _DEBUG
		ImguiCtrl::Terminate();
#endif
		return Terminate();
	}

	int ArApp::Terminate() const
	{
		//arGfx->Terminate();
		return 0;
	}

	void ArApp::Quit()
	{
		quit = true;
	}

}
