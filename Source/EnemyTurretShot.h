#pragma once
#include "Argent/Argent.h"

class EnemyTurretShot :
    public Argent::Component::BaseActor
{
public:
    // コンストラクタ
    EnemyTurretShot(const char* name, DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f })
        :BaseActor(name), init_position(pos) {}
    // デストラクタ
    ~EnemyTurretShot() {}

    void Initialize()override;  // 最初の一回だけ呼ばれる
    void Update()override;      // 毎フレームbeginとendの間に呼ばれる
    void DrawDebug()override;

    void SetVelocity(DirectX::XMFLOAT3 velo) { velocity = velo; }
    DirectX::XMFLOAT3 GetVelocity() { return velocity; }

    //// test
    //void SetTransfrom(Transform t) { transform = t; }


    // 移動処理
    void UpdateMove();



public:
    float eraseTimer = {};  // 弾が消えるまでの時間

private:
    DirectX::XMFLOAT3 velocity{};
    DirectX::XMFLOAT3 moveVec{};
    DirectX::XMFLOAT3 angle{};

    Transform transform{};

private:
    // 初期座標位置
    DirectX::XMFLOAT3 init_position{};
};

