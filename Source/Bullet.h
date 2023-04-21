#pragma once
#include "Argent/Argent.h"

/**
 * \brief 弾丸の基底クラス　
 */
class Bullet:
	public Argent::Component::BaseActor
{
public:
	/**
	 * \brief 
	 * \param direction 進行方向
	 * \param damage ダメージ
　	 * \param speed 弾丸の速度 
	 */
	Bullet(const DirectX::XMFLOAT3& direction, float damage, float speed);
	virtual ~Bullet() override = default;


	void Initialize() override;
	void Update() override;

	static void Shot(Bullet* bulletActor, const Transform& t);

	void DrawDebug() override;

protected:
	float damage;
	float speed;
	DirectX::XMFLOAT3 direction;
	Argent::Component::Collision::RayCast* ray; 
};

