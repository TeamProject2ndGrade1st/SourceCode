#pragma once
#include "BaseState.h"

namespace Friend::Creature
{
    class IdleState : public FriendState
    {
    public:
        IdleState(BaseFriend* _friend) : FriendState(_friend, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    //叫び　基本的に色々な行動の予備動作になる
    class ActionState : public FriendState
    {
    public:
        ActionState(BaseFriend* _friend) : FriendState(_friend, "action") {}
        ~ActionState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    //前進
    class WalkState : public FriendState
    {
    public:
        WalkState(BaseFriend* _friend) : FriendState(_friend, "walk") {}
        ~WalkState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    private:
        float acceleration{ 1000.0f };
        float maxSpeed_fast{ 300.0f };
        float maxSpeed_late{ 80.0f };
        float maxSpeed{ 200.0f };

        //アニメーション20~32フレーム間で動きが早くなる
        int startMovingFastFrame{ 20 };
        int endMovingFastFrame{ 32 };
    };

    class AttackState : public FriendState
    {
    public:
        AttackState(BaseFriend* _friend) : FriendState(_friend, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    private:
        float attackInterval{ 3.0f };
        int efeStartFrame{ 27 };

        float hitStopTime{ 0.27f };

        bool didAttack{};
    };

    //階層構造のステート（多分つかわん）
    class H_MoveState : public HierarchicalState
    {
    public:
        H_MoveState(BaseFriend* _friend) : HierarchicalState(_friend) {}
        ~H_MoveState();

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    class H_BattleState : public HierarchicalState
    {
    public:
        H_BattleState(BaseFriend* _friend) : HierarchicalState(_friend) {}
        ~H_BattleState();

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };
}

namespace Friend::Drone 
{
    class IdleState : public FriendState
    {
    public:
        IdleState(BaseFriend* _friend) : FriendState(_friend, "idle") {}
        ~IdleState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;
    };

    //前進
    class WalkState : public FriendState
    {
    public:
        WalkState(BaseFriend* _friend) : FriendState(_friend, "walk") {}
        ~WalkState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    private:
        float acceleration{ 1000.0f };
        float maxSpeed{ 200.0f };

        //アニメーション20~32フレーム間で動きが早くなる
        int startMovingFastFrame{ 20 };
        int endMovingFastFrame{ 32 };
    };

    class AttackState : public FriendState
    {
    public:
        AttackState(BaseFriend* _friend) : FriendState(_friend, "attack") {}
        ~AttackState() {}

        void Enter()override;
        void Execute()override;
        void Exit()override;

    private:
        float attackInterval{ 3.0f };
        bool didAttack{};
    };

}
