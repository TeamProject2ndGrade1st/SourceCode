#include "Title.h"
#include <sstream>

#include "Argent/Component/ArSpriteRenderer.h"
#include "Argent/Input/Keyboard.h"
#include "Argent/Component/ArEffectRenderer.h"
#include "Argent/Component/ArAudioPlayer.h"
#include "Argent/Loader/FbxLoader.h"
#include "MrIncredible.h"
#include "Argent/Component/Collider.h"
#include "BaseBullet.h"


void Title::Initialize()
{
	AddObject(new GameObject("Demo"));
	GetGameObject("Demo")->AddComponent(new Argent::Component::Renderer::ArSpriteRenderer("./Resources/Image/Sample256.png"));

	//AddObject(GameObject::Sphere());
	//AddObject( new GameObject("obj",new ArSkinnedMeshRenderer(Argent::Graphics::ArGraphics::Instance()->GetDevice(), "./Resource/DragonBinary.fbx")));

	//AddObject(new GameObject("noneBone", Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG/LMG_low_0324_1.fbx")));

	AddObject(new GameObject("stage01", Argent::Loader::Fbx::LoadFbx("./Resources/Model/map_0402_ver5.fbx", false)));

	AddObject(new GameObject("DestroyDemo", new BaseBullet(DirectX::XMFLOAT3(), 0, 0)));
	//AddObject(new GameObject("stage01", Argent::Loader::Fbx::LoadFbx("./Resources/Model/kaidan_0330_ver1.fbx", true)));

	//AddObject(new GameObject("stageblend", Argent::Loader::Fbx::LoadFbx("./Resources/Model/tmpStages.fbx", false)));

	//AddObject(new GameObject("effect", new Argent::Component::Renderer::ArEffectRenderer("./Resources/Effects/barel_test.efk ", "./Resources/Effects")));

	//AddObject(new GameObject("Nico", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));
	//GetGameObject("Nico")->AddComponent(new MrIncredible);
	//GetGameObject("Nico")->AddComponent(new Argent::Component::Collider::SphereCollider);

	//AddObject(new GameObject("Inc", Argent::Loader::Fbx::LoadFbx("./Resources/Model/Mr.Incredible.fbx", false)));
	//GetGameObject("Inc")->AddComponent(new MrIncredible);
	//GetGameObject("Inc")->AddComponent(new Argent::Component::Collider::SphereCollider);
	  
	BaseScene::Initialize();
}

void Title::Finalize()
{
	BaseScene::Finalize();
}

void Title::Update()
{
	BaseScene::Update();

	if(Argent::Input::Mouse::Instance().IsButtonPress(Argent::Input::Mouse::Mouses::mLeftButton))
	{
		//SceneManager::SetNextScene("Game");
	}
}

void Title::Render()
{
	BaseScene::Render();
}

#ifdef _DEBUG
void Title::DrawDebug()
{
	BaseScene::DrawDebug();
}
#endif

