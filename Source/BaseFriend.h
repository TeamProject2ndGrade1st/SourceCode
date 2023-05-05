#pragma once
#include <DirectXMath.h>

#include "Argent/Argent.h"
#include "Character.h"
#include "StateMachine.h"
#include "FriendManager.h"
#include "BaseEnemy.h"
#include "EnemyManager.h"

class BaseFriend : public Character
{
public:
    enum Route
    {
        left,
        center,
        right
    };

    //�i�s���[�g��ŕK���ʂ�|�C���g
    struct  RelayPoint
    {
        DirectX::XMFLOAT3 pos;
        bool passage;//�ʉ߂�����
    };

    BaseFriend(const char* name, DirectX::XMFLOAT3 pos = {0,0,0},Route route = Route::left);
    virtual ~BaseFriend() {}

    //BaseFriend(DirectX::XMFLOAT3 pos);


    virtual void Initialize()override;
    virtual void Begin()override;
    virtual void Update()override;
    void DrawDebug() override;

    //�^�[�Q�b�g�Ɍ������Ĉړ�
    void MoveToTarget();

    void SetAnimation(int index);
    bool IsAnimationEnd()
    {
        return GetOwner()->GetComponent<Argent::Component::Renderer::SkinnedMeshRenderer>()->IsAnimationEnd();
    }

    virtual void OnDamaged() override;//��_���[�W���ɌĂ΂��
    virtual void OnDead() override;//���S���ɌĂ΂��
    virtual void OnHeal() override;//�񕜎��ɌĂ΂��

    //�^�[�Q�b�g���U���͈͓��ɂ��邩
    bool IsTargetInAttackArea();
    bool SerchEnemy();

    void SetStateTimer(float timer) { stateTimer = timer; }
    float GetStateTimer() const { return stateTimer; }
    void SetAttackTimer(float timer) { attackTimer = timer; }
    float GetAttackTimer() const { return attackTimer; }

    //�����l�Q�b�^�[�A�Z�b�^�[
    float Init_GetAccelaration() const { return init_acceleration; }
    void Init_SetAccelaration(float accel) { init_acceleration = accel; }
    float Init_GetMaxMoveSpeed() const { return init_maxMoveSpeed; }
    void Init_SetMaxMoveSpeed(float speed) { init_maxMoveSpeed = speed; }
    float Init_GetFriction() const { return init_friction; }
    void Init_SetFriction(float friction) { init_friction = friction; }

    
    //DirectX::XMFLOAT3 GetTargetPosition() const { return target->GetOwner()->GetTransform()->GetPosition(); }
    float GetAttackAreaRadius() const { return attackAreaRadius; }

    StateMachine* GetStateMachine() const { return stateMachine.get(); }
    BaseEnemy* GetTarget() const { return target; }

    void RouteSearch(std::vector<RelayPoint>& point,Route routeNo);

protected:
    //�ڕW���W
    BaseEnemy* target{ nullptr };

    //�U���n���͈�
    float attackAreaRadius{};
    float init_attackAreaRadius{ 300.0f };
  
    //�����l
    float init_acceleration{ 0.0f };
    float init_maxMoveSpeed{ 500.0f };
    float init_friction{ 500.0f };

    float stateTimer{};

    //�U����̃C���^�[�o���v���p
    float attackTimer{};

    std::unique_ptr<StateMachine> stateMachine{ nullptr };
    EnemyManager* eManager{ nullptr };

    //Update�ň�x�����Ă΂��
    bool once;

    std::vector<RelayPoint> relayPoint;
};

