#pragma once
#include <vector>
#include "Argent/Argent.h"
#include "BaseBullet.h"

class BaseBulletManager
{
public:
    BaseBulletManager();
    virtual ~BaseBulletManager() = default;

    void Shot(BaseBullet* bulletActor);
protected:
    std::vector<GameObject*> bullets;
};

