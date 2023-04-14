#pragma once

#include <xaudio2.h>
#include <x3daudio.h>
#include <wrl.h>


namespace Argent::Resource::Audio
{
	class AudioManager
	{
	public:
		AudioManager();
		virtual ~AudioManager() = default;


		IXAudio2* GetAudioEngine() const { return audioEngine.Get();  }
		IXAudio2MasteringVoice* GetMasterVoice() const { return masterVoice; }
	private:
		Microsoft::WRL::ComPtr<IXAudio2> audioEngine;
		IXAudio2MasteringVoice* masterVoice{};

	public:
		static AudioManager& Instance()
		{
			static AudioManager ins;
			return ins;
		}

	private:
		static AudioManager* instance;
	public:
		//static AudioManager* Instance()
		//{
		//	return instance;
		//}
	};
	
}

