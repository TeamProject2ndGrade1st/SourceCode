#include "AnimationPlayer.h"
#include "../Core/Timer.h"

namespace Argent::Component::Animation
{
	AnimationPlayer::AnimationPlayer(std::vector<Resource::Animation::AnimationClip> clips):
		BaseComponent("Animation Player")
	{
		this->clips = clips;
	}

	void AnimationPlayer::Update()
	{
		if (clips.size() == 0) return;
		
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
	}
}