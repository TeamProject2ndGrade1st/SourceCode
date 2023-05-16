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

    //衝撃を受ける(ノックバック)
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //ダメージを受ける
    bool ApplyDamage(float damage);
    //回復を受ける
    bool ApplyHeal(float heal);

    float GetAttack()const { return attack; }

    void HitStop(float t) { GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->SetStopTime(t); }

    void SetActiveState(bool active) { activeState = active; }

protected:
    //速力処理(水平のみ)
    void UpdateVelocity();

    //移動処理
    void UpdateMove();

    //旋回処理
    void Turn(float vx, float vz,float rollSpeed);

    virtual void OnDamaged() {}//被ダメージ時に呼ばれる
    virtual void OnDead() {}//死亡時に呼ばれる
    virtual void OnHeal() {}//回復時に呼ばれる



protected:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};

    float acceleration{ 300.0f };//加速力
    float maxMoveSpeed{ 500.0f };//最高速度
    float friction{ 200.0f };//摩擦力

    float maxHealth = { 5.0f };//体力
    float health = { maxHealth };//体力
    float attack = { 1.0f };//攻撃力

    float weight = { 30.0f };//体重(最大で100)

    GameObject* myShadow{ nullptr };

    bool activeState{true};

private:
    //初期座標
    DirectX::XMFLOAT3 init_position{};

    //なんか座標の初期化がinitializeから
    //begin処理の間でリセットされるから一旦無理やりbeginに初期座標入れとく
    bool initOnce;
};

