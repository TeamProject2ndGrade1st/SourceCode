#pragma once
#include "Argent/Argent.h"


/**
 * \brief �e�ۂ̊��N���X�@
 */
class Bullet:
	public Argent::Component::BaseActor
{
public:
	/**
	 * \brief 
	 * \param direction �i�s����
	 * \param damage �_���[�W
�@	 * \param speed �e�ۂ̑��x 
	 */
	Bullet(const DirectX::XMFLOAT3& direction, int damage, float speed);
	virtual ~Bullet() override = default;


	void Initialize() override;
	void Update() override;


	/**
	 * \brief �e�۔��˗p�̊֐�
	 * \param position ���˒n�_
	 * \param direction ���˕����i�i�s�����j
	 * \param damage �_���[�W
	 * \param speed �i�s���x
	 */
	static void Shot(const DirectX::XMFLOAT3& position,
	                 const DirectX::XMFLOAT3& direction, int damage, float speed);

	void DrawDebug() override;

protected:
	int damage;
	float speed;
	DirectX::XMFLOAT3 direction;
	Argent::Component::Collision::RayCast* ray;

	float elapsedTime;
	inline static float validTime = 5.0f;
};

