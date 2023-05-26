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

//一桁の数字
class OneDigit : public Argent::Component::BaseComponent
{
public:
    OneDigit(int num = 0) : BaseComponent("OneDigit"), number(num) {}
    ~OneDigit() {}

    void Initialize()override;
    void Update()override;

    //代入処理、引数のtimeは数字が固定されるまでの演出の長さ
    void Assign(int num,float time);

    //演出のための関数、実際の数値は変更されないが見た目のみ変更する
    void DrawNum(int num);

private:
    int number;
    float timer;

    //各数字に応じた配列（画像の切り抜きが今はできないので仕方なく各数字に分けて出力する）
    std::vector<GameObject*> numbers;
};

//複数の桁数に対応したクラス
class Number : public Argent::Component::BaseComponent
{
public:
    Number(int* num,int digit/*桁数*/) : BaseComponent("Number"), number(num),digits(digit) {}
    ~Number() {}

    void Initialize()override;
    void Update()override;

    void Assign(int num);

private:
    int* number;

    //桁数を固定する
    const int digits;

    std::vector<GameObject*> numbers;
};