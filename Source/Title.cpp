#include "Title.h"

#include "Argent/Argent.h"
#include "BaseBullet.h"
#include "BaseGun.h"

void Title::Initialize()
{
	//AddObject(new GameObject("Demo"));
	//GetGameObject("Demo")->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png"));

	//AddObject(new GameObject("noneBone", Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG/LMG_low_0324_1.fbx")));

	//AddObject(new GameObject("stage01", Argent::Loader::Fbx::LoadFbx("./Resources/Model/map_0402_ver5_original_blender.fbx", false)));
	AddObject(new GameObject("plane", Argent::Loader::Fbx::LoadFbx("./Resources/Model/plane.fbx", true)));

	//AddObject(new GameObject("DestroyDemo", new BaseBullet(DirectX::XMFLOAT3(), 0, 0)));
	//AddObject(new GameObject("handGun", Argent::Loader::Fbx::LoadFbx("./Resources/Model/HG_1_0310_ver1.fbx", true)));

	//AddObject(new GameObject("lightMachineGun", Argent::Loader::Fbx::LoadFbx("./Resources/Model/LMG_high_0326_3.fbx", true)));
	AddObject(new GameObject("enemy_ver03", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver3.fbx", false)));

	AddObject(new GameObject("DemoGun", new BaseGun()));
	//AddObject(new GameObject("enemy_ver1", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001_ver1.fbx", true)));
	//AddObject(new GameObject("enemy_ver1", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));

	//AddObject(new GameObject("effect", new Argent::Component::Renderer::ArEffectRenderer("./Resources/Effects/barel_test.efk ", "./Resources/Effects")));

	AddObject(new GameObject("Nico", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));
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

