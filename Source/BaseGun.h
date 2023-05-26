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
	void EarlyUpdate() final;
	void Finalize() override;

	void DrawDebug() override;

	//���������̏e�̗h��Ƃ��A�������Ƃ��̔����Ƃ�
	void RecoilUpdate();
	void TremorUpdate();
	void AddRecoil(DirectX::XMFLOAT3 power,float downTime);

	Bullet::Mode GetMode() { return mode; }
	float fireRate;
	int damage;
	float speed;

	Bullet::Mode mode;
	GameObject* lmg;

	//�����Ƃ��ɂ��U���p
	DirectX::XMFLOAT3 tremorMove;
	float tremorSpeed{800};
	//�e�̐U�ꕝ
	float tremorAmp{0.12f};
	//���C�Ƃ��̔����p
	DirectX::XMFLOAT3 recoilMove;
	DirectX::XMFLOAT3 recoilPower;
#ifdef _DEBUG
	bool enableShot = true;

#endif
	bool walking{ nullptr };

	//�����ɂ��U����\�����邽�߂̕ϐ�
	float tremorWalk;
	float recoilUpTimer;
	float recoilDownTimer;

	//IMGUI�p
	float recoilUpTime{0.03f};
	float recoilDownTime{0.05f};

	DirectX::XMFLOAT3 offset;
};