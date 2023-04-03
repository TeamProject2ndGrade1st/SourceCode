#pragma once
#include "Argent/Scene/BaseScene.h"

class Game final:
    public Argent::Scene::BaseScene
{
public:
    Game(const std::string& sceneName):
        Argent::Scene::BaseScene(sceneName)
    {}
    ~Game() override = default;


    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Render() override;

private:
};

