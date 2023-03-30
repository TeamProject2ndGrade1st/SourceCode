#pragma once
#include "Argent/Component/BaseActor.h"


class MrIncredible:
	public Argent::Component::BaseActor
{
public:
	MrIncredible();
	~MrIncredible() override = default;


	void Initialize() override;
	void Update() override;
	void OnCollision(const Argent::Component::Collider::Collider* collider) override;

#ifdef _DEBUG
	void DrawDebug() override;
#endif
protected:
	float moveSpeed;
};

