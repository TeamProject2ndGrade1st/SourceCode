#pragma once
#include "Argent/Argent.h"
class Character :
    public Argent::Component::BaseActor
{
public:
    Character(const char* name, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f }) 
        :BaseActor(name) ,init_position(pos) {}
    virtual ~Character() {}

    void Initialize()override;
    void DrawDebug()override;
    
    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }
    void SetAccelaration(float accel) { acceleration = accel; }
    void SetMaxSpeed(float speed) { maxMoveSpeed = speed; }
    void SetFriction(float fric) { friction = fric; }

    //�Ռ����󂯂�(�m�b�N�o�b�N)
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //�_���[�W���󂯂�
    bool ApplyDamage(float damage);
    //�񕜂��󂯂�
    bool ApplyHeal(float heal);

protected:
    //���͏���(�����̂�)
    void UpdateVelocity();

    //�ړ�����
    void UpdateMove();

    //���񏈗�
    void Turn(float vx, float vz,float rollSpeed);

    virtual void OnDamaged() {}//��_���[�W���ɌĂ΂��
    virtual void OnDead() {}//���S���ɌĂ΂��
    virtual void OnHeal() {}//�񕜎��ɌĂ΂��

protected:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};

    float acceleration{ 3.0f };//������
    float maxMoveSpeed{ 5.0f };//�ō����x
    float friction{ 2.0f };//���C��

    float maxHealth = { 5.0f };//�̗�
    float health = { maxHealth };//�̗�
    float attack = { 1.0f };//�U����

    float weight = { 30.0f };//�̏d(�ő��100)


private:
    //�������W
    DirectX::XMFLOAT3 init_position{};
};

