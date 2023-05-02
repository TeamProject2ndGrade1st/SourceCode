#include "Game.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Demo.h"

#include "Stage.h"

#include "FriendCreature.h"
#include "Player.h"
#include "Argent/Debug/DebugRenderer.h"
#include "Demo.h"
#include "EnemyTurret.h"
#include "Reticle.h"

#include "EnemyManager.h"
#include "EnemySpikeBot.h"
#include "EnemyTurret.h"



void Game::Initialize()
{
	ClearGameObject();
	Argent::Loader::Fbx::LoadDebug("./Resources/Model/Collision/Cube.fbx");
	Argent::Loader::Fbx::LoadDebug("./Resources/Model/Collision/Sphere.fbx");
	auto g = GameObject::Instantiate("UI", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/UI.png"));
	g->GetTransform()->SetPosition(DirectX::XMFLOAT3(250, 615, 0));
	//AddObject(new GameObject("Demo", new Argent::Component::Renderer::SpriteRenderer("./Resources/Image/Sample256.png")));
	
	//AddObject(new GameObject("EffectSlash", new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/slash.efk", "./Resources/Effects")));
	GameObject::Instantiate("Reticle", new Reticle("./Resources/Image/ReticleYellow.png"));
	//AddObject(new GameObject("player", Argent::Loader::Fbx::LoadFbx("./Resources/Model/ene_1_0410_ver4.fbx")));
	// player ‚Â‚¢‚©
	AddObject(new GameObject("player", new Player));
	//GetGameObject("player")->AddComponent(new Player);
//	AddObject(new GameObject("player", new Player));

	GameObject::Instantiate("Effect", new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/barel_test.efk", "./Resources/Effects"));

	//AddObject(new GameObject("spikeBot", Argent::Loader::Fbx::LoadFbx("./Resources/Model/spike_bot_0419_1.fbx")));
	
	AddObject(new GameObject("spikebot", new EnemySpikeBot()));
	AddObject(new GameObject("turret", new EnemyTurret()));

	//AddObject(new GameObject("friend", new FriendCreature()));
	AddObject(new GameObject("FriendManager", new FriendManager(this)));
	AddObject(new GameObject("EnemyManager", new EnemyManager(this)));

	GameObject::Instantiate("Main Stage", new Stage("./Resources/Model/Stage/map_2_0427_3.fbx"));
	GameObject::Instantiate("Box", new Box("./Resources/Model/Stage/boxes_0419_1.fbx"));
	GameObject::Instantiate("OwnCamp", new OwnCamp("./Resources/Model/Stage/zizin_0419_1.fbx"));
	GameObject::Instantiate("Core", new Core("./Resources/Model/Stage/core_0419_1.fbx"));
	GameObject::Instantiate("HoneyComb", new Honeycomb("./Resources/Model/Stage/hanikamu_0425_1.fbx"));
	GameObject::Instantiate("Barricade", new Barricade("./Resources/Model/Stage/barike-do_0419_1.fbx"));
	GameObject::Instantiate("Tutorial Stage", new TutorialStage("./Resources/Model/Stage/map_1_0426_5.fbx"));

	BaseScene::Initialize();
	std::vector<GameObject*> lightArray;
	GameObject::FindByTag(GameObject::Tag::Light, lightArray);
	lightArray.at(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, -10.0f, -50));
	lightArray.at(1)->GetTransform()->SetPosition(DirectX::XMFLOAT3(-80.0f, 10.0f, 50.0f));
	lightArray.at(2)->GetTransform()->SetPosition(DirectX::XMFLOAT3(35.0f, 18.0f, 30.0f));

	std::vector<GameObject*> camera;
	GameObject::FindByTag(GameObject::Tag::MainCamera, camera);
	camera.at(0)->GetTransform()->SetPosition(DirectX::XMFLOAT3(0, 27.0f, -500.0f));

	Argent::Input::Mouse::Instance().resetPositionToCenter = true;

	GameObject::Instantiate("Effect", new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/shield_bash.efk", "./Resources/Effects/"));

#ifndef _DEBUG
	ShowCursor(false);
#endif
}

void Game::Finalize()
{
	ShowCursor(true);
	Argent::Input::Mouse::Instance().resetPositionToCenter = false;
	BaseScene::Finalize();
}

void Game::Update()
{
#ifdef _DEBUG
	static bool b = false;
	if(Argent::Input::GetKeyUp(KeyCode::F4))
	{
		b = !b;
		ShowCursor(b);
	}
#else
	ShowCursor(false);
#endif
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
