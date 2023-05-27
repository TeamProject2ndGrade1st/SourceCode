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
    Argent::Component::Renderer::SpriteRenderer* lifeGre;
    Argent::Component::Renderer::SpriteRenderer* elecGre;
    Argent::Component::Renderer::SpriteRenderer* lifeAmo;
    Argent::Component::Renderer::SpriteRenderer* elecAmo;
    Argent::Component::Renderer::SpriteRenderer* huel;

    Argent::Component::Renderer::SpriteRenderer* edit;

    Argent::Component::Renderer::SpriteRenderer* tutorial_shopOpen;
    Argent::Component::Renderer::SpriteRenderer* tutorial_move;
    Argent::Component::Renderer::SpriteRenderer* tutorial_unit;
    Argent::Component::Renderer::SpriteRenderer* tutorial_shop;
    Argent::Component::Renderer::SpriteRenderer* tutorial_friend;
    Argent::Component::Renderer::SpriteRenderer* tutorial_setFriend;

    Player* player;
    BaseGun* gun;
    int* friendNum;
    int* enemyNum;

    Shop* shop;
    ChangeMode* mode;

    Reticle* reticle;
};

