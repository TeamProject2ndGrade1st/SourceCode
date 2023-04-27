#pragma once
#include "Argent/Argent.h"
#include "FriendManager.h"


/**
 * \brief �e�ۂ̊��N���X�@
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
	 * \param direction �i�s����
	 * \param damage �_���[�W
�@	 * \param speed �e�ۂ̑��x 
	 */
	Bullet(const DirectX::XMFLOAT3& direction, int damage, float speed, Mode mode);
	virtual ~Bullet() override = default;


	void Initialize() override;
	void Update() override;
	void DrawDebug() override;

	/**
	 * \brief �e�۔��˗p�̊֐�
	 * \param position ���˒n�_
	 * \param direction ���˕����i�i�s�����j
	 * \param damage �_���[�W
	 * \param speed �i�s���x
	 */
	static void Shot(const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& direction, int damage, float speed,
		Mode mode);

	/**
	 * \brief ���ݎ��Ԃ̊Ǘ��@�������ꍇ�͂��̃N���X�����L����Q�[���I�u�W�F�N�g��j�󂷂�
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

	//���݂̃V�[������t�����h�}�l�[�W���[���擾����
	static void GetFriendManagerFromScene();
};

