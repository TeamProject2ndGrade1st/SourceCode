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

//�ꌅ�̐���
class OneDigit : public Argent::Component::BaseComponent
{
public:
    OneDigit(int num = 0) : BaseComponent("OneDigit"), number(num) {}
    ~OneDigit() {}

    void Initialize()override;
    void Update()override;

    //��������A������time�͐������Œ肳���܂ł̉��o�̒���
    void Assign(int num,float time);

    //���o�̂��߂̊֐��A���ۂ̐��l�͕ύX����Ȃ��������ڂ̂ݕύX����
    void DrawNum(int num);

private:
    int number;
    float timer;

    //�e�����ɉ������z��i�摜�̐؂蔲�������͂ł��Ȃ��̂Ŏd���Ȃ��e�����ɕ����ďo�͂���j
    std::vector<GameObject*> numbers;
};

//�����̌����ɑΉ������N���X
class Number : public Argent::Component::BaseComponent
{
public:
    Number(int* num,int digit/*����*/) : BaseComponent("Number"), number(num),digits(digit) {}
    ~Number() {}

    void Initialize()override;
    void Update()override;

    void Assign(int num);

private:
    int* number;

    //�������Œ肷��
    const int digits;

    std::vector<GameObject*> numbers;
};