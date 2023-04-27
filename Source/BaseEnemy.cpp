#include "BaseEnemy.h"
#include "EnemyStateDerivad.h"

// �R���X�g���N�^
BaseEnemy::BaseEnemy(const char* name, DirectX::XMFLOAT3 pos) :
    Character(name, pos)
{
}

void BaseEnemy::Initialize()
{
    
    if (!fManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::FriendManager, g);
        if (g.size() > 0)
            fManager = g.at(0)->GetComponent<FriendManager>();
    }
    //GetOwner()->AddComponent(new Argent::Component::Collider::RayCastCollider(Argent::Component::Collider::RayCastCollider::MeshType::Cube));
    
    
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->offset.y = 1.0f;
    //GetOwner()->GetComponent < Argent::Component::Collider::RayCastCollider>()->scale = {
    //    attackAreaRadius 100.0f,attackAreaRadius * 100.0f,attackAreaRadius * 100.0f
    //};
    


}

void BaseEnemy::Begin()
{
    if (!fManager)
    {
        std::vector<GameObject*> g;
        GameObject::FindByTag(GameObject::Tag::FriendManager, g);
        if (g.size() > 0)
            fManager = g.at(0)->GetComponent<FriendManager>();
    }
}

void BaseEnemy::Update()
{


    
}

void BaseEnemy::DrawDebug()
{
    if (ImGui::TreeNode(GetName()))
    {
        ImGui::TreePop();
    }
}

void BaseEnemy::addspeed()
{
    // �X�s�[�h���o�Ă���
    if (friendAddSpeed > 0)
    {
        //auto f = GetFriend();
        //DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();
        //friendPos.z -= friendAddSpeed;
        //f->GetTransform()->SetPosition(friendPos);
        //friendAddSpeed -= 0.1f;

        
        
    }
    

    // �X�s�[�h�����̏I��
    if (friendAddSpeed < 0)friendAddSpeed = 0;
}

BaseFriend* BaseEnemy::SearchFriend1()
{
    // TODO: return �X�e�[�g�����g�������ɑ}�����܂�
        // Friend�^�O���t���Ă���GameObject��T��
    std::vector<GameObject*> Friend;
    GameObject::FindByTag(GameObject::Tag::Friend, Friend);

    DirectX::XMFLOAT3 pos = GetOwner()->GetTransform()->GetPosition();
    DirectX::XMFLOAT4 angle = GetOwner()->GetTransform()->GetRotation();

    // TO DO �r��
    for (auto f : Friend)
    {
        // Friend �� position�����
        DirectX::XMFLOAT3 friendPos = f->GetTransform()->GetPosition();

        float vx = pos.x - friendPos.x;
        float vy = pos.y - friendPos.y;
        float vz = pos.z - friendPos.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < 50)
        {
            float distXZ = sqrtf(vx * vx + vz * vz);
            // �P�ʃx�N�g����
            vx /= distXZ;
            vz /= distXZ;

            // �����x�N�g����
            float frontX = sinf(angle.y);
            float frontZ = cosf(angle.y);
            // 2�̃x�N�g���̓��ϒl�őO�㔻��
            float dot = (frontX * vx) + (frontZ * vz);
            if (dot > 0.0f)
            {
                f->GetTransform()->SetPosition(friendPos);
                auto* ret = fManager->FindFriendComponentFromOwner(f);
                return ret;
                //return f;
            }
        }
    }
    return nullptr;
}
