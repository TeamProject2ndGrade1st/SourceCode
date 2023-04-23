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
	Bullet(const DirectX::XMFLOAT3& direction, int damage, float speed);
	virtual ~Bullet() override = default;


	void Initialize() override;
	void Update() override;
	void DrawDebug() override;

	/**
	 * \brief 弾丸発射用の関数
	 * \param position 発射地点
	 * \param direction 発射方向（進行方向）
	 * \param damage ダメージ
	 * \param speed 進行速度
	 */
	static void Shot(const DirectX::XMFLOAT3& position,
	                 const DirectX::XMFLOAT3& direction, int damage, float speed);

	/**
	 * \brief 存在時間の管理　超えた場合はこのクラスを所有するゲームオブジェクトを破壊する
	 */
	void ManageDuration();
	void Move();
	void OnCollision();
protected:
	int damage;
	float speed;
	DirectX::XMFLOAT3 direction;
	Argent::Component::Collision::RayCast* ray;

	float elapsedTime;
	inline static float validTime = 5.0f;
};

