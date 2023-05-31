#pragma once
#include "Argent/Argent.h"
#include "StateMachine.h"

class Tutorial : public Argent::Component::BaseActor
{
public:

    enum State
    {
        tutorial_0,
        tutorial_1,
        tutorial_2,
        tutorial_3,
        tutorial_4,
        tutorial_5,
        tutorial_6,
        end,
    };

    Tutorial():BaseActor("Tutorial") {}
    ~Tutorial() {}

    void Initialize()override;
    void Update()override;

    void StateUpdate();

private:
    GameObject* explanation[end];

    int state;

    bool init[end];
};

