#pragma once
#include "Argent/Argent.h"

class Shadow;

class Character :
    public Argent::Component::BaseActor
{
public:
    Character(const char* name, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f }) 
        :BaseActor(name) ,init_position(pos) {}
    virtual ~Character() {}

    void Initialize()override;
    void DrawDebug()override;
    void Begin()override;
    
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

    float GetAttack()const { return attack; }

    void HitStop(float t) { GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->SetStopTime(t); }

    void SetActiveState(bool active) { activeState = active; }

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

    float acceleration{ 300.0f };//������
    float maxMoveSpeed{ 500.0f };//�ō����x
    float friction{ 200.0f };//���C��

    float maxHealth = { 5.0f };//�̗�
    float health = { maxHealth };//�̗�
    float attack = { 1.0f };//�U����

    float weight = { 30.0f };//�̏d(�ő��100)

    GameObject* myShadow{ nullptr };

    bool activeState{true};

private:
    //�������W
    DirectX::XMFLOAT3 init_position{};

    //�Ȃ񂩍��W�̏�������initialize����
    //begin�����̊ԂŃ��Z�b�g����邩���U�������begin�ɏ������W����Ƃ�
    bool initOnce;
};

