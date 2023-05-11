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

	//•à‚¢‚½‚Ìe‚Ì—h‚ê‚Æ‚©AŒ‚‚Á‚½‚Æ‚«‚Ì”½“®‚Æ‚©
	void RecoilUpdate();
	void AddRecoil(DirectX::XMFLOAT3 power);

	float fireRate;
	int damage;
	float speed;

	DirectX::XMFLOAT3 offset{ -3.0f, -3.0f, 3.0f };

	Bullet::Mode mode;
	GameObject* lmg;

	//•à‚«‚Æ‚©ŠÉ‚¢U“®—p
	DirectX::XMFLOAT3 tremorMove;
	float tremorSpeed{360};
	//e‚ÌU‚ê•
	float tremorAmp{0.5f};
	//”­–C‚Æ‚©‚Ì”½“®—p
	DirectX::XMFLOAT3 recoilMove;
#ifdef _DEBUG
	bool enableShot = true;

#endif
	bool walking{ nullptr };

	//•à‚«‚É‚æ‚éU“®‚ğ•\Œ»‚·‚é‚½‚ß‚Ì•Ï”
	float tremorWalk;
};