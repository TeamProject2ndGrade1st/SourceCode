#pragma once
#include "Component.h"
#include "Color.h"

class Light :
    public Argent::Component::BaseComponent
{
public:
    Light(std::string name = "Light", DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1));
	virtual ~Light() = default;


    void Reset() override;
    void End() override;
#ifdef _DEBUG
    void DrawDebug() override;
#endif
    [[nodiscard]] Argent::ArColor GetColor() const { return color; }


protected:
    Argent::ArColor color;
};

