#include "Title.h"

#include "Argent/Argent.h"

#include "BaseFriend.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Demo.h"
#include "Stage.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"
#include "spikeBot.h"
#include "FriendCreature.h"


void Title::Initialize()
{
	ClearGameObject();

	//AddObject(new GameObject("plane", Argent::Loader::Fbx::LoadFbx("./Resources/Model/plane.fbx", true)));
	
	//AddObject(new GameObject("enemy_ver03", Argent::Loader::Fbx::LoadFbx("./Resources/Model/enemy_001Ver3.fbx", false)));

	//AddObject(new GameObject("nico", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));
	//AddObject(new GameObject("friend", new BaseFriend()));

	
	//AddObject(new GameObject("DemoGun", Argent::Loader::Fbx::LoadFbx("./Resources/Model/StageBlender.fbx", false)));

	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Plane.fbx");
	
	//AddObject(new GameObject("Nico", new RayCastDemo()));
//	AddObject(new GameObject("Gun", new BaseGun));
	//AddObject(new GameObject("Stage", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));


	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	//AddObject(new GameObject("player", new Player));
	//GetGameObject("player")->AddComponent(new Player);
//	AddObject(new GameObject("player", new Player));

	// spikeBot
	//AddObject(new GameObject("spikeBot", Argent::Loader::Fbx::LoadFbx("./Resources/Model/spike_bot_0419_1.fbx")));
	//AddObject(new GameObject("spikeBot", new SpikeBot()));
	AddObject(new GameObject("EnemySpikeBot", new EnemySpikeBot));
	AddObject(new GameObject("EnemyTurret", new EnemyTurret));

	// ‰¼‚ÌGameObject
	AddObject(new GameObject("spikeBot", new SpikeBot));
	//AddObject(new GameObject("Creature", new FriendCreature));

	//AddObject(new GameObject("Demo", new RayCastDemo));


	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/barike-do_0419_1.fbx")));
	GetGameObject("player")->AddComponent(new Player);
	//AddObject(new GameObject("Stage1", Argent::Loader::Fbx::LoadFbx("./Resources/Model/capsule.fbx", true)));
	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Cube.fbx");
	//AddObject(new GameObject("Debug", new Argent::Debug::Debug()));


//	AddObject(new GameObject("MeshDemo", Argent::Loader::Fbx::LoadFbx("./Resources/Model/StageBlender.fbx")));

	//AddObject(new GameObject("MeshDemo", Argent::Loader::Fbx::LoadFbx("./Resources/Model/Stage/map_0419_1.fbx")));


//	AddObject(new GameObject("Nico", new RayCastDemo()));
//	AddObject(new GameObject("Debug", new Argent::Component::Collider::RayCastCollider()));
	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	//AddObject(new GameObject("capsuel", Argent::Loader::Fbx::LoadFbx("./Resources/Model/capsule.fbx", true)));
	//GetGameObject("capsuel")->GetTransform()->SetScaleFactor(0.01f);
	//Argent::Loader::Fbx::LoadDebug("./Resources/Model/Cube.fbx");
	//AddObject(new GameObject("Debug", new Argent::Debug::Debug()));
	//AddObject(new GameObject("Debug", new Argent::Component::Collider::RayCastCollider()));


	AddObject(new GameObject("Stage", new Stage));


	//AddObject(new GameObject("Stage", new Stage));

	AddObject(new GameObject("TitleSprite", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Title.png")));


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

	if(Argent::Input::GetKeyUp(KeyCode::F2))
	{
		Argent::Scene::SceneManager::SetNextScene("StageSelect");
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

