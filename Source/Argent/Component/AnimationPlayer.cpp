#include "AnimationPlayer.h"
#include "../Core/Timer.h"

void Argent::Component::Animation::AnimationPlayer::Update()
{
	if (clips.size() == 0) return;
	//static float animationTick{};
	const Resource::Animation::AnimationClip& animation{ this->clips.at(clipIndex) };
	frameIndex = static_cast<float>(animationTick* animation.samplingRate);
	if(frameIndex > animation.sequence.size() - 1)
	{
		frameIndex = 0;

		animationTick = 0;
	}
	else
	{
		animationTick += Timer::ArTimer::Instance().DeltaTime();
	}
	BaseComponent::Update();
}
