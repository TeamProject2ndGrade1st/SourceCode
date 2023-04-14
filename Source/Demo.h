#pragma once
#include "Argent/Argent.h"

class RayCastDemo:
	public Argent::Component::BaseActor
{
public:
	RayCastDemo():
		BaseActor("RayCastDemo")
	{}

	void Initialize() override;

	void Update() override;

	void DrawDebug() override;

	Argent::Component::Collision::RayCast* ray;
	Argent::Component::Audio::AudioPlayer* audio;
	float moveSpeed;
};
