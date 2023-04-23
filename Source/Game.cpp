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


void Game::Initialize()
{
	ClearGameObject();
	//AddObject(new GameObject("Demo", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png")));

	GameObject::Instantiate("Reticle", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/ReticleRed.png"));
	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	AddObject(new GameObject("player", new Player));
	//GetGameObject("player")->AddComponent(new Player);
//	AddObject(new GameObject("player", new Player));

	// spikeBot
	//AddObject(new GameObject("spikeBot", Argent::Loader::Fbx::LoadFbx("./Resources/Model/spike_bot_0419_1.fbx")));
	AddObject(new GameObject("spikeBot", new SpikeBot()));

	AddObject(new GameObject("friend", new FriendCreature()));
	AddObject(new GameObject("target", Argent::Loader::Fbx::LoadFbx("./Resources/Model/nico.fbx", false)));
	AddObject(new GameObject("FriendManager", new FriendManager(this)));

	AddObject(new GameObject("Stage", new Stage));

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
