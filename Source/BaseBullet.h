#pragma once
#include "Argent/Component/BaseActor.h"

/**
 * \brief 弾丸の基底クラス　
 */
class BaseBullet:
	public Argent::Component::BaseActor
{
public:
	/**
	 * \brief 
	 * \param direction 進行方向
	 * \param damage ダメージ
　	 * \param speed 弾丸の速度 
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

