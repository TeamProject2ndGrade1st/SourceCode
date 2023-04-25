#include "Game.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Demo.h"

#include "Stage.h"

#include "spikeBot.h"


#include "FriendCreature.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Demo.h"
#include "EnemyTurret.h"



void Game::Initialize()
{
	ClearGameObject();
	//AddObject(new GameObject("Demo", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png")));

	GameObject::Instantiate("Reticle", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/ReticelYellow.png"));
	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	AddObject(new GameObject("player", new Player));
	//GetGameObject("player")->AddComponent(new Player);
//	AddObject(new GameObject("player", new Player));

	GameObject::Instantiate("Effect", new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/barel_test.efk", "./Resources/Effects"));

	//AddObject(new GameObject("spikeBot", Argent::Loader::Fbx::LoadFbx("./Resources/Model/spike_bot_0419_1.fbx")));
	//AddObject(new GameObject("spikeBot", new SpikeBot()));

	//AddObject(new GameObject("turret", new EnemyTurret()));

	//AddObject(new GameObject("friend", new FriendCreature()));
	auto* g = new GameObject("target", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false));
	AddObject(g);
	auto* hoge0 = new GameObject("shadow", Argent::Loader::Fbx::LoadFbx("./Resources/Model/shadow.fbx", false));
	AddObject(hoge0);
	auto* hoge2 = new GameObject("shadow_ver2", Argent::Loader::Fbx::LoadFbx("./Resources/Model/shadow_ver2.fbx", false));
	AddObject(hoge2);
	AddObject(new GameObject("FriendManager", new FriendManager(this)));

	GameObject::Instantiate("Main Stage", new Stage("./Resources/Model/Stage/map_0419_1.fbx"));
	GameObject::Instantiate("Box", new Box("./Resources/Model/Stage/boxes_0419_1.fbx"));
	GameObject::Instantiate("OwnCamp", new OwnCamp("./Resources/Model/Stage/zizin_0419_1.fbx"));
	GameObject::Instantiate("Core", new Core("./Resources/Model/Stage/ore.fbx"));

	//AddObject(new GameObject("Main Stage", new Stage("./Resources/Model/Stage/map_0419_1.fbx")));
	//AddObject(new GameObject("Box", new Stage("./Resources/Model/Stage/boxes_0419_1.fbx")));
	//AddObject(new GameObject("Core", new Stage("./Resources/Model/Stage/core_0419_1.fbx")));
	//AddObject(new GameObject("Own Camp", new Stage("./Resources/Model/Stage/zizin_0419_1.fbx")));
	//AddObject(new GameObject("Tutorial Stage", new Stage("./Resources/Model/StageBlender.fbx")));


	BaseScene::Initialize();
}

void Game::Finalize()
{
	BaseScene::Finalize();
}

void Game::Update()
{
	if(Argent::Input::GetKeyUp(KeyCode::F2))
	{
		Argent::Scene::SceneManager::SetNextScene("Result");
	}

	if(Argent::Input::GetKeyUp(KeyCode::Q))
	{
		Argent::App::Quit();
	}
	BaseScene::Update();
}

void Game::Render()
{
	BaseScene::Render();
}
