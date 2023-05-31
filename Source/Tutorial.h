#pragma once
#include "Argent/Argent.h"
#include "StateMachine.h"
#include "ChangeMode.h"

class Tutorial : public Argent::Component::BaseActor
{
public:

    enum State
    {
        Move,
        OpenShop,
        Shop1,
        Shop2,
        ChangeEdit,
        Edit,
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

    ChangeMode* mode;
};

