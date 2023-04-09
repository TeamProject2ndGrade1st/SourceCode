#include "App.h"
#include "../../../imgui/ImguiCtrl.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Input/Keyboard.h"
#include "../Component/ArColliderManager.h"

namespace Argent::App
{
	ArApp::ArApp(HINSTANCE hInstance, LONG width, LONG height, const char* appName, bool isFullScreen)
	{
		static bool isInstantiated{ FALSE };
		if(isInstantiated)_ASSERT_EXPR(FALSE, L"Instance is already existed");

		arWindow = std::make_unique<Window::ArWindow>(hInstance, width, height);
		arGfx = std::make_unique<Graphics::ArGraphics>(arWindow->GetHandle());
		effectManager = std::make_unique<Argent::Resource::Effect::EffectManager>(arGfx->GetDevice(), arGfx->GetCommandQueue(), arGfx->GetNumBackBuffers());

		isInstantiated = true;
	}

	void ArApp::Initialize() const
	{
		arGfx->Initialize();
		ImguiCtrl::Initialize(arWindow->GetHandle(), arGfx->GetDevice(), arGfx->GetGUIHeap());
		Resource::ResourceManager::Instance().Initialize();
	}

	int ArApp::Execute() const
	{
		Initialize();
			
		Scene::ArSceneManager arSceneManager;
		arSceneManager.Initialize();
		while (MainLoop(arWindow->GetHandle()))
		{
			Argent::Input::Keyboard::Instance().Update();

			ImguiCtrl::Begin("Main Window");
			arSceneManager.Begin();

			arSceneManager.Update();
			effectManager->Update();

			Argent::Collider::ArColliderManager::Instance().CollisionDetection();

			arGfx->Begin();
			arSceneManager.Render();
			effectManager->Render();

			arSceneManager.End();
			ImguiCtrl::End(arGfx->GetCommandList(), arGfx->GetGUIHeap());

			arGfx->End();
			arSceneManager.DeleteDestroyedObject();
		}
		arSceneManager.Finalize();
		ImguiCtrl::Terminate();
		return Terminate();
	}

	int ArApp::Terminate() const
	{
		arGfx->Terminate();
		return 0;
	}
}