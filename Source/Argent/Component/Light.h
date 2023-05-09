#pragma once
#include "Component.h"
#include "Color.h"
#include "../Graphic/SceneConstant.h"

class DirectionalLight :
    public Argent::Component::BaseComponent
{
public:
    DirectionalLight(int index, std::string name = "Light", DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(1, 1, 1), float power = 1.0f);
	virtual ~DirectionalLight() override = default;

    void Reset() override;
    void Render() const override;

    void DrawDebug() override;
    DirectX::XMFLOAT3 GetColor() const { return color; }


protected:
   // Argent::Color color;
    DirectX::XMFLOAT3 color;
    float power;
    DirectX::XMFLOAT3 direction;
    int index;
};

class PointLight:
    public Argent::Component::BaseComponent
{
public:
    PointLight(int index, float range = 50.0f):
		BaseComponent("PointLight")
    ,   index(index)
    ,   range(range)
    {}

    ~PointLight() override = default;

    void Initialize() override;
    void Render() const override;
    void DrawDebug() override;

private:
    DirectX::XMFLOAT3 color;
    float power;
   // Argent::Color color;
    float range;
    int index;
};