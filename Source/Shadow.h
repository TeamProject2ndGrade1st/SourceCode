#pragma once
#include "Argent/Argent.h"
#include "Character.h"
class Shadow :
    public Argent::Component::BaseActor
{
public:
    Shadow(Character* p) : parent(p),BaseActor("CharaShadow") {}
    ~Shadow() {}

    void Initialize()override;
    void Update()override;

private:
    Character* parent{ nullptr };
};

