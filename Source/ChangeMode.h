#pragma once

#include "Argent/Argent.h"

class ChangeMode : public Argent::Component::BaseComponent
{
public:
    ChangeMode():BaseComponent("BattleMode") {}
    ~ChangeMode() {}

    void Initialize()override;
    void Update()override;

    void ChangeBattleMode();

    //èÄîıÉÇÅ[Éh
    void ChangeReadyMode();

    bool OpenShop();
    void CloseShop();

    bool battleFlag{false};
    bool openShop{false};
};

