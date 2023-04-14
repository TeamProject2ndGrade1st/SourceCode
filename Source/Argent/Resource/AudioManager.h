#pragma once

#include <xaudio2.h>
#include <x3daudio.h>
#include <wrl.h>
#include "../Resource/Audio.h"
class Transform;

namespace Argent::Resource::Audio
{
	class AudioManager
	{
	public:
		AudioManager();
		virtual ~AudioManager() = default;


		IXAudio2* GetAudioEngine() const { return audioEngine.Get();  }
		IXAudio2MasteringVoice* GetMasterVoice() const { return masterVoice; }
		void Initialize();

		void Update();
		void DrawDebug();
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
		X3DAUDIO_HANDLE X3DInstance;
	public:
		//static AudioManager* Instance()
		//{
		//	return instance;
		//}

		Transform* emitterPos;
		Transform* listenerPos;
		std::shared_ptr<Argent::Resource::Audio::AudioResource> audio;
		IXAudio2SubmixVoice* submixVoice;
		IUnknown* reverbEffect;
		UINT32 nChannel;
		FLOAT32 matrixCoefficients[1 * 8];
		DWORD dwChannelMask;
		float outputMatrix[8 * 2];
		float pan = 0;
		float volume = 1.0f;
	};
	
}

