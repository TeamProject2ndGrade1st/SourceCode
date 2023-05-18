#pragma once

#include "Argent/Argent.h"

class ChangeMode : public Argent::Component::BaseComponent
{
public:
    enum class Mode
    {
        Ready,
        Edit,
        Battle,
    };

    ChangeMode():BaseComponent("BattleMode") {}
    ~ChangeMode() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void ChangeBattleMode();

    //�������[�h
    void ChangeReadyMode();

    //�ݒu���[�h
    void ChangeEditMode();

    bool OpenShop();
    void CloseShop();

    bool battleFlag{false};
    bool openShop{false};

    bool canChange{ true };

    Mode currentMode{Mode::Ready};
};

