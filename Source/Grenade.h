#pragma once
#include "Argent/Argent.h"
#include "Character.h"
#include "FriendManager.h"

class Grenade:
	public Argent::Component::BaseActor
{
public:
	enum Mode
	{
		Creature,
		Machine,
	};

	Grenade(Mode mode, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& forward):
		BaseActor("Grenade")
	,	mode(mode)
	,	initialPosition(position)
	{
		direction = forward;
		DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)));
	};

	~Grenade() override = default;

	void Initialize() override;

	void Update() override;
	void DrawDebug() override;
	//É_ÉÅÅ[ÉWì¸ÇÍÇÈÇ‚Ç¬
	void OnDamage(Character* target, GameObject* targetOwner);

	void GenerateEffect(const DirectX::XMFLOAT3& pos);
private:
	FriendManager* friendManager;
	float range = 100;
	int damage;
	Mode mode;
	bool onExplode;
	static float validTime;
	float elapsedTime;

	static constexpr float gravity = 200.0f;
	float speed = 200.0f;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 velocity;

	DirectX::XMFLOAT3 initialPosition;
#ifdef _DEBUG
	Argent::Component::Collider::RayCastCollider* debugRenderer;
#endif

	std::vector<Character*> judgedCharacter{};
};

