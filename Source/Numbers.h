#pragma once
#include "Argent/Argent.h"

//�ꌅ�̐���
class OneDigit : public Argent::Component::BaseActor
{
public:
    OneDigit(float offsetX,int num = 0) : BaseActor("OneDigit"), offsetX(offsetX), number(num) {}
    ~OneDigit() {}

    void Initialize()override;
    void Update()override;
    void Finalize()override { BaseActor::Finalize(); }

    //��������A������time�͐������Œ肳���܂ł̉��o�̒���
    void Assign(int num, float time);

    //���o�̂��߂̊֐��A���ۂ̐��l�͕ύX����Ȃ��������ڂ̂ݕύX����
    void DrawNum(int num);

    float offsetX;

private:
    int number;
    float timer;

    //�e�����ɉ������z��i�摜�̐؂蔲�������͂ł��Ȃ��̂Ŏd���Ȃ��e�����ɕ����ďo�͂���j
    std::vector<Argent::Component::Renderer::SpriteRenderer*> numbers;
};

//�����̌����ɑΉ������N���X
class Number : public Argent::Component::BaseActor
{
public:
    Number(int* num, int digit/*����*/) : BaseActor("Number"), number(num),currentNum(*num), digits(digit) {}
    ~Number() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void Assign(int num);

private:
    int* number;

    int currentNum;

    //�������Œ肷��
    const int digits;

    std::vector<OneDigit*> numbers;
};

