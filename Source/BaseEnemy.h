#pragma once
#include <DirectXMath.h>
#include "Argent/Argent.h"
#include "Character.h"
#include "StateMachine.h"


class BaseEnemy : public Character
{
public:
    BaseEnemy();
    virtual ~BaseEnemy() {}

    virtual void Initialize()override;
    virtual void Begin()override;
    virtual void Update()override;
    virtual void DrawDebug()override;

protected:
    
};