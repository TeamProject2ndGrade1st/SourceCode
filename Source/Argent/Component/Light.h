#pragma once
#include "Component.h"
#include "Color.h"

class Light :
    public Argent::Component::BaseComponent
{
public:
    Light(int index, std::string name = "Light", DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1));
	virtual ~Light() = default;


    void Initialize() override;
    void Reset() override;
    void End() override;

    void DrawDebug() override;
    [[nodiscard]] Argent::Color GetColor() const { return color; }


protected:
    Argent::Color color;
    int index;
};

