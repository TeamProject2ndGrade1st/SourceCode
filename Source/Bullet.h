#pragma once
#include "Argent/Argent.h"
#include "FriendManager.h"


/**
 * \brief 弾丸の基底クラス　
 */
class Bullet:
	public Argent::Component::BaseActor
{
public:
	enum class Mode
	{
		Living,
		Machine
	};

	/**
	 * \brief 
	 * \param direction 進行方向
	 * \param damage ダメージ
　	 * \param speed 弾丸の速度 
	 */
	Bullet(const DirectX::XMFLOAT3& direction, int damage, float speed, Mode mode);
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
		const DirectX::XMFLOAT3& direction, int damage, float speed,
		Mode mode);

	/**
	 * \brief 存在時間の管理　超えた場合はこのクラスを所有するゲームオブジェクトを破壊する
	 */
	void ManageDuration();
	void Move();
	void OnCollision(const HitResult& result);
	static void ClearFriendManager() { friendManager = nullptr; }
protected:

	void AddDamage(BaseFriend* f);

	int damage;
	float speed;
	DirectX::XMFLOAT3 direction;
	Argent::Component::Collision::RayCast* ray;

	float elapsedTime;
	Mode mode;


	inline static float validTime = 5.0f;
	inline static FriendManager* friendManager = nullptr;

	//現在のシーンからフレンドマネージャーを取得する
	static void GetFriendManagerFromScene();
};

