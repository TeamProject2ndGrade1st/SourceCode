#pragma once
#include <vector>
#include "Argent/Argent.h"
#include "BaseBullet.h"

class BaseBulletManager
{
    BaseBulletManager() = default;
public:
    static BaseBulletManager& Instance()
    {
        static BaseBulletManager instance;
        return instance;
    }
    virtual ~BaseBulletManager() = default;

protected:
};

