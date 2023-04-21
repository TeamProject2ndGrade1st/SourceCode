#pragma once
#include "Argent/Scene/BaseScene.h"

class Game final:
    public Argent::Scene::BaseScene
{
public:
    Game():
        Argent::Scene::BaseScene("Game")
    {}
    ~Game() override = default;


    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Render() override;

private:
};

