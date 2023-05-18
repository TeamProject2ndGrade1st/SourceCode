#pragma once
#include "Argent/Argent.h"
#include "Player.h"
#include "Shop.h"
#include "Reticle.h"

class UI : public Argent::Component::BaseActor
{
public:
    UI():BaseActor("UI") {}
    ~UI() {}

    void Initialize()override;
    void Update()override;

private:
    Argent::Component::Renderer::SpriteRenderer* base;
    Argent::Component::Renderer::SpriteRenderer* hp;
    Argent::Component::Renderer::SpriteRenderer* lifeGure;
    Argent::Component::Renderer::SpriteRenderer* elecGure;
    Argent::Component::Renderer::SpriteRenderer* lifeAmo;
    Argent::Component::Renderer::SpriteRenderer* elecAmo;
    Argent::Component::Renderer::SpriteRenderer* huel;

    Player* player;
    BaseGun* gun;
    int* friendNum;
    int* enemyNum;

    Shop* shop;
    ChangeMode* mode;

    Reticle* reticle;
};

