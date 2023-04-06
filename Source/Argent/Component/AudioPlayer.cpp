#include "AudioPlayer.h"
#include "../Other/Helper.h"
#include "../Input/Keyboard.h"


namespace Argent::Component::Audio
{
	AudioPlayer::AudioPlayer(const char* filePath):
		BaseComponent(Helper::String::ExtractFileName(filePath, false))
	{
		audio = std::make_shared<Argent::Resource::Audio::AudioResource>(filePath);
	}

	void AudioPlayer::Update()
	{
		if(Argent::Input::Keyboard::Instance().GetKeyDown(Input::Keyboard::KeyCode::Space))
		{
			audio->Play();
		}
	}

}