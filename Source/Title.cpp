#include "Title.h"

#include "Argent/Argent.h"
#include "BaseBullet.h"
#include "BaseGun.h"
#include "BaseFriend.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Argent/Component/RayCast.h"
#include "Demo.h"

void Title::Initialize()
{
	AddObject(new GameObject("Demo", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png")));

	//AddObject(new GameObject("plane", Argent::Loader::Fbx::LoadFbx("./Resources/Model/plane.fbx", true)));
	
	//AddObject(new GameObject("enemy_ver03", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver3.fbx", false)));

	//AddObject(new GameObject("nico", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));
	AddObject(new GameObject("enemy_ver03", new BaseFriend()));

	//AddObject(new GameObject("DemoGun", Argent::Loader::Fbx::LoadFbx("./Resources/Model/StageBlender.fbx", false)));

	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Plane.fbx");
	
	//AddObject(new GameObject("Nico", new RayCastDemo()));
//	AddObject(new GameObject("Gun", new BaseGun));
	//AddObject(new GameObject("Stage", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));

//	AddObject(new GameObject("player", new Player));
	AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	GetGameObject("player")->AddComponent(new Player);
	//AddObject(new GameObject("Stage1", Argent::Loader::Fbx::LoadFbx("./Resources/Model/capsule.fbx", true)));
	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Cube.fbx");
	//AddObject(new GameObject("Debug", new Argent::Debug::Debug()));


	AddObject(new GameObject("Nico", new RayCastDemo()));
	AddObject(new GameObject("Debug", new Argent::Component::Collider::RayCastCollider()));
	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	AddObject(new GameObject("capsuel", Argent::Loader::Fbx::LoadFbx("./Resources/Model/capsule.fbx", true)));
	GetGameObject("capsuel")->GetTransform()->SetScaleFactor(0.01f);
	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Cube.fbx");
	//AddObject(new GameObject("Debug", new Argent::Debug::Debug()));
	//AddObject(new GameObject("Debug", new Argent::Component::Collider::RayCastCollider()));


	BaseScene::Initialize();
}

void Title::Begin()
{
	BaseScene::Begin();
}

void Title::Finalize()
{
	BaseScene::Finalize();
}



void Title::Update()
{
	BaseScene::Update();

	if(Argent::Input::GetKeyDown(KeyCode::P))
	{
		Argent::Scene::ArSceneManager::SetNextScene("Game");
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

