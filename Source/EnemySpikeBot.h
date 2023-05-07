#pragma once
#include "BaseEnemy.h"
#include "Argent/Argent.h"

enum class SpikeBotAnimation
{
    Idle,
    Attack,
};

class EnemySpikeBot :
    public BaseEnemy
{
public:
    EnemySpikeBot(DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f })
        :BaseEnemy("EnemySpikeBot", pos) {}
    ~EnemySpikeBot() {}

    void Initialize()override;
    void Begin()override;
    void Update()override;
    void DrawDebug()override;

    enum class State
    {
        Idle,
        Attack,
    };

public:
    Argent::Component::Renderer::EffekseerEmitter* effect;
};

