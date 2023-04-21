#pragma once
#include "BaseState.h"


class EnemyIdleState : public FriendState
{
public:
    // �R���X�g���N�^
    EnemyIdleState(BaseFriend* _friend) :FriendState(_friend, "idle") {}
    // �f�X�g���N�^
    ~EnemyIdleState() {}


    void Enter()override;
    void Execute()override;
    void Exit()override;

    bool SearchPlayer();

    
};