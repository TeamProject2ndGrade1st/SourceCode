#include "EnemyStateDerivad.h"
#include "Argent/Argent.h"
#include "BaseFriend.h"


void EnemyIdleState::Enter()
{

}

void EnemyIdleState::Execute()
{

}

void EnemyIdleState::Exit()
{

}

bool EnemyIdleState::SearchPlayer()
{
    // �^�O��ǉ�����
    owner->GetOwner()->SetTag(GameObject::Tag::SpikeBot);
    
    //GameObject::FindByTag()



    // �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
    //const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
    //float vx = playerPosition.x - position.x;
    //float vy = playerPosition.y - position.y;
    //float vz = playerPosition.z - position.z;
    //float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    //if (dist < searchRange)
    //{
    //	float distXZ = sqrtf(vx * vx + vz * vz);
    //	// �P�ʃx�N�g����
    //	vx /= distXZ;
    //	vz /= distXZ;

    //	// �����x�N�g����
    //	float frontX = sinf(angle.y);
    //	float frontZ = cosf(angle.y);
    //	// 2�̃x�N�g���̓��ϒl�őO�㔻��
    //	float dot = (frontX * vx) + (frontZ * vz);
    //	if (dot > 0.0f)
    //	{
    //		return true;
    //	}
    //}
    return false;
}