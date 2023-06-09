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
	void TremorUpdate();
	void AddRecoil(DirectX::XMFLOAT3 power,float downTime);

	Bullet::Mode GetMode() { return mode; }
	float fireRate;
	int damage;
	float speed;

	Bullet::Mode mode;
	GameObject* lmg;

	//歩きとか緩い振動用
	DirectX::XMFLOAT3 tremorMove;
	float tremorSpeed{800};
	//銃の振れ幅
	float tremorAmp{0.12f};
	//発砲とかの反動用
	DirectX::XMFLOAT3 recoilMove;
	DirectX::XMFLOAT3 recoilPower;
#ifdef _DEBUG
	bool enableShot = true;

#endif
	bool walking{ nullptr };

	//歩きによる振動を表現するための変数
	float tremorWalk;
	float recoilUpTimer;
	float recoilDownTimer;

	//IMGUI用
	float recoilUpTime{0.03f};
	float recoilDownTime{0.05f};

	DirectX::XMFLOAT3 offset;
};