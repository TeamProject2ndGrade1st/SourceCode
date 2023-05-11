#pragma once
#include "Argent/Argent.h"
#include "Bullet.h"

class BaseGun :
	public Argent::Component::BaseComponent
{
public:
	BaseGun(const char* name);
	virtual ~BaseGun() override = default;

	void Initialize() override;
	void Update() final;
	void Finalize() override;

	void DrawDebug() override;

	//歩いた時の銃の揺れとか、撃ったときの反動とか
	void RecoilUpdate();
	void AddRecoil(DirectX::XMFLOAT3 power);

	float fireRate;
	int damage;
	float speed;

	DirectX::XMFLOAT3 offset;

	Bullet::Mode mode;
	GameObject* lmg;

	//歩きとか緩い振動用
	DirectX::XMFLOAT3 tremorMove;
	//発砲とかの反動用
	DirectX::XMFLOAT3 recoilMove;
#ifdef _DEBUG
	bool enableShot = true;

#endif
	bool walking{ nullptr };

	//歩きによる振動を表現するための変数
	float tremorWalk;
};