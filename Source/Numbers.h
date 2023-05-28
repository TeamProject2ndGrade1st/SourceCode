#pragma once
#include "Argent/Argent.h"

//一桁の数字
class OneDigit : public Argent::Component::BaseActor
{
public:
    OneDigit(float offsetX,int num = 0) : BaseActor("OneDigit"), offsetX(offsetX), number(num) {}
    ~OneDigit() {}

    void Initialize()override;
    void Update()override;
    void Finalize()override { BaseActor::Finalize(); }

    //代入処理、引数のtimeは数字が固定されるまでの演出の長さ
    void Assign(int num, float time);

    //演出のための関数、実際の数値は変更されないが見た目のみ変更する
    void DrawNum(int num);

    float offsetX;

private:
    int number;
    float timer;

    //各数字に応じた配列（画像の切り抜きが今はできないので仕方なく各数字に分けて出力する）
    std::vector<Argent::Component::Renderer::SpriteRenderer*> numbers;
};

//複数の桁数に対応したクラス
class Number : public Argent::Component::BaseActor
{
public:
    Number(int* num, int digit/*桁数*/) : BaseActor("Number"), number(num),currentNum(*num), digits(digit) {}
    ~Number() {}

    void Initialize()override;
    void Update()override;
    void DrawDebug()override;

    void Assign(int num);

private:
    int* number;

    int currentNum;

    //桁数を固定する
    const int digits;

    std::vector<OneDigit*> numbers;
};

