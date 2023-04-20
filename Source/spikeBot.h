#pragma once
#include "Argent/Argent.h"

class SpikeBot:
    public Argent::Component::BaseActor
{
public:
    SpikeBot();
    virtual ~SpikeBot()override = default;

    void Initialize()override;
    void Update()override;

    void DrawDebug()override;

private:


};

