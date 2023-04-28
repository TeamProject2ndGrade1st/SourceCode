#pragma once
#include "Argent/Argent.h"
#include "Character.h"
class muzzleFlash :
    public Argent::Component::BaseActor
{
public:
    muzzleFlash(Character* p) : parent(p), BaseActor("CharaMuzzleFlash") {}
    ~muzzleFlash() {}

    void Initialize()override;
    void Update()override;

private:
    Character* parent{ nullptr };
};
#pragma once
