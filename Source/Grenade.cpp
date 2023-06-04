#include "Grenade.h"

#include "BaseFriend.h"


float Grenade::validTime = 0.5f;


void Grenade::Initialize()
{
	friendManager = nullptr;
	if(mode == Mode::Creature)
	{
		GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/pack_0424_3.fbx"));
	}
	else
	{
		GetOwner()->AddComponent(Argent::Loader::Fbx::LoadFbx("./Resources/Model/kongure_0511_3.fbx"));
		GetOwner()->GetTransform()->SetRotation(DirectX::XMFLOAT4(0, 90, 0, 0));
	}
	std::vector<GameObject*> friendManagers;
	if(GameObject::FindByTag(GameObject::Tag::FriendManager, friendManagers))
	{
		friendManager = friendManagers.at(0)->GetComponent<FriendManager>();
	}
	onExplode = false;

#ifdef _DEBUG
	debugRenderer = new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Sphere);
	GetOwner()->AddComponent(debugRenderer);
#endif

	judgedCharacter.clear();
	GetOwner()->GetTransform()->SetPosition(initialPosition);
	velocity = direction * speed;
}

void Grenade::Update()
{
	if(!friendManager)
	{
		std::vector<GameObject*> friendManagers;
		if(GameObject::FindByTag(GameObject::Tag::FriendManager, friendManagers))
		{
			friendManager = friendManagers.at(0)->GetComponent<FriendManager>();
		}
	}

#ifdef _DEBUG
	debugRenderer->scale = DirectX::XMFLOAT3(range / 2, range / 2, range/ 2);
#endif

	//座標更新
	if(!onExplode)
	{
		auto p  = GetOwner()->GetTransform()->GetPosition();
		//p = p + direction * speed * Argent::Timer::GetDeltaTime();
		//p.y -= gravity * Argent::Timer::GetDeltaTime();
		velocity.y -= gravity * Argent::Timer::GetDeltaTime();
		p = p + velocity * Argent::Timer::GetDeltaTime();
		GetOwner()->GetTransform()->SetPosition(p);
	}


#if 0
	if(Argent::Input::GetKeyDown(KeyCode::G))
	{
		GenerateEffect(DirectX::XMFLOAT3(0, 10, 0));
	}
#endif

	if(GetOwner()->GetTransform()->GetPosition().y < 5)
	{
		onExplode = true;
	}


	if(onExplode)
	{
		//爆発ダメージor回復
		DirectX::XMFLOAT3 pos = GetOwner()->GetTransform()->GetPosition();
		for(size_t i = 0; i < friendManager->friendArray.size(); ++i)
		{
			Character* target = friendManager->friendArray.at(i);
			GameObject* targetOwner = friendManager->friendArray.at(i)->GetOwner();

			//ゲームオブジェクトがアクティブが確認
			if (!targetOwner)continue;
			if(!targetOwner->GetIsActive()) continue;

			 
			//距離判定
			if(Length(pos, targetOwner->GetTransform()->GetPosition()) < range)
			{
				//すでに判定していないかのチェック
				bool onJudged = false;
				for(auto& c : judgedCharacter)
				{
					if(c == target)
					{
						onJudged = true;
						break;
					}
				}
				//一度判定していた場合はしない
				if(onJudged) continue;

				//ダメージ判定 & エフェクトの再生
				OnDamage(target, targetOwner);
				//判定済みの配列に追加
				judgedCharacter.emplace_back(target);
			}
		}

		//有効時間の確認
		elapsedTime += Argent::Timer::GetDeltaTime();
		if(elapsedTime > validTime) GameObject::Destroy(GetOwner());
	}
}

void Grenade::DrawDebug()
{
	if(ImGui::TreeNode(GetName()))
	{
		ImGui::SliderFloat("Range", &range, 0.1f, 1000.0f);
		ImGui::SliderInt("Damage", &damage, 0, 10);
		ImGui::SliderFloat("ElapsedTime", &elapsedTime, 0, 100);

		ImGui::TreePop();
	}
}

void Grenade::OnDamage(Character* target, GameObject* targetOwner)
{
	unsigned tag = targetOwner->GetUnsignedTag();
	if(mode == Mode::Creature)
	{
		if(tag & static_cast<unsigned>(GameObject::Tag::Creature))
		{
			target->ApplyHeal(damage);
			GenerateEffect(targetOwner->GetTransform()->GetPosition());
		}
		else if(tag & static_cast<unsigned>(GameObject::Tag::Machine))
		{
			target->ApplyDamage(damage);
		}
	}
	else
	{
		if(tag & static_cast<unsigned>(GameObject::Tag::Creature))
		{
			target->ApplyDamage(damage);
		}
		else if(tag & static_cast<unsigned>(GameObject::Tag::Machine))
		{
			target->ApplyHeal(damage);
			GenerateEffect(targetOwner->GetTransform()->GetPosition());
		}
	}
}

void Grenade::GenerateEffect(const DirectX::XMFLOAT3& pos)
{
	//todo エフェクトの再生
	auto e = new Argent::Component::Renderer::EffekseerEmitter("./Resources/Effects/heal.efk", "./Resources/Effects");
	auto g = GameObject::Instantiate("RegenEffect", e);
	g->GetTransform()->SetPosition(pos);
	e->scale = DirectX::XMFLOAT3(10, 10, 10);
	e->OnPlay();
}
