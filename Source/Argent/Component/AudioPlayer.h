#pragma once

#include "Component.h"
#include <memory>
#include "../Resource/Audio.h"


namespace Argent::Component::Audio
{
	class AudioPlayer:
		public ArComponent
	{
	public:
		AudioPlayer(const char* filePath);
		~AudioPlayer() override = default;

		void Update() override;

	private:
		std::shared_ptr<Argent::Resource::Audio::ArAudio> audio;

	};
}
