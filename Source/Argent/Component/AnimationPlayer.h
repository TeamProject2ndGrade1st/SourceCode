#pragma once
#include "Component.h"
#include "../Resource/Animation.h"

namespace Argent::Component::Animation
{
	class AnimationPlayer :
		public BaseComponent
	{
		AnimationPlayer();

		~AnimationPlayer() override = default;

		void Update() override;

		Resource::Animation::AnimationClip GetCurrentClip() const { return clips.at(clipIndex); }
		Resource::Animation::AnimationClip::Keyframe GetCurrentKeyFrame() const { return clips.at(clipIndex).sequence.at(static_cast<int>(frameIndex)); }
		void SetClipIndex(int i) { clipIndex = i; }
	protected:
		std::vector<Resource::Animation::AnimationClip> clips{};
		int clipIndex;
		float frameIndex;
		float animationTick;
	};



}
