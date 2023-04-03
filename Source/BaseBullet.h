#pragma once
#include "Argent/Component/BaseActor.h"

/**
 * \brief �e�ۂ̊��N���X�@
 */
class BaseBullet:
	public Argent::Component::BaseActor
{
public:
	/**
	 * \brief 
	 * \param direction �i�s����
	 * \param damage �_���[�W
�@	 * \param speed �e�ۂ̑��x 
	 */
	BaseBullet(const DirectX::XMFLOAT3& direction, float damage, float speed);
	virtual ~BaseBullet() override = default;

	void OnCollision(const Argent::Component::Collider::Collider* collider) override;

	void Initialize() override;
	void Update() override;

#ifdef _DEBUG
	void DrawDebug() override;
#endif
protected:
	float damage;
	float speed;
	DirectX::XMFLOAT3 direction;
};

