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

