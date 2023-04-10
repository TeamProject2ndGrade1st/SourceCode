#include "Title.h"

#include "Argent/Argent.h"
#include "BaseBullet.h"
#include "BaseGun.h"

void Title::Initialize()
{
	//AddObject(new GameObject("Demo"));
	//GetGameObject("Demo")->AddComponent(new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png"));

	//AddObject(new GameObject("plane", Argent::Loader::Fbx::LoadFbx("./Resources/Model/plane.fbx", true)));

	//AddObject(new GameObject("enemy_ver03", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver3.fbx", false)));

	AddObject(new GameObject("DemoGun", new BaseGun()));

	AddObject(new GameObject("Nico", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));

	AddObject(new GameObject("Stage", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	AddObject(new GameObject("Stage1", Argent::Loader::Fbx::LoadFbx("./Resources/Model/capsule.fbx", true)));
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

