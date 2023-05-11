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

	//���������̏e�̗h��Ƃ��A�������Ƃ��̔����Ƃ�
	void RecoilUpdate();
	void AddRecoil(DirectX::XMFLOAT3 power);

	float fireRate;
	int damage;
	float speed;

	DirectX::XMFLOAT3 offset;

	Bullet::Mode mode;
	GameObject* lmg;

	//�����Ƃ��ɂ��U���p
	DirectX::XMFLOAT3 tremorMove;
	//���C�Ƃ��̔����p
	DirectX::XMFLOAT3 recoilMove;
#ifdef _DEBUG
	bool enableShot = true;

#endif
	bool walking{ nullptr };

	//�����ɂ��U����\�����邽�߂̕ϐ�
	float tremorWalk;
};