#pragma once
#include "BaseState.h"


class EnemyIdleState : public FriendState
{
public:
    // コンストラクタ
    EnemyIdleState(BaseFriend* _friend) :FriendState(_friend, "idle") {}
    // デストラクタ
    ~EnemyIdleState() {}


    void Enter()override;
    void Execute()override;
    void Exit()override;

    bool SearchPlayer();

    
};