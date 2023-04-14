#include "AudioManager.h"
#include "../Other/Misc.h"

namespace Argent::Resource::Audio
{
	AudioManager* AudioManager::instance = nullptr;
	AudioManager::AudioManager()
	{
		if (instance) _ASSERT_EXPR(FALSE, "already instanciated");
		instance = this;


		HRESULT hr{ S_OK };

		//オーディオエンジンの作成
		hr = XAudio2Create(audioEngine.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//マスターボイスの作成
		hr = audioEngine->CreateMasteringVoice(&masterVoice);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		DWORD dwChannelMask;
		masterVoice->GetChannelMask(&dwChannelMask);
		X3DAUDIO_HANDLE X3DInstance;
		//X3DAudio
		hr = X3DAudioInitialize(dwChannelMask, 
			X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		 
		X3DAUDIO_LISTENER listener = {};
		X3DAUDIO_EMITTER emitter = {};
		emitter.ChannelCount = 1;
		emitter.CurveDistanceScaler = emitter.DopplerScaler = 1.0f;

		X3DAUDIO_DSP_SETTINGS DSPSettings = {};
		DSPSettings.SrcChannelCount = emitter.ChannelCount;
		DSPSettings.DstChannelCount = emitter.ChannelCount;
		DSPSettings.pDelayTimes = nullptr;
		//XAUDIO2_VOICE_DETAILS voiceDetail{};
		//masterVoice->GetVoiceDetails(&voiceDetail);
		DSPSettings.pMatrixCoefficients = nullptr;

		X3DAudioCalculate(X3DInstance, &listener, &emitter, NULL, &DSPSettings);

		//3dオーディオ設定を計算
		//emitter.OrientFront = Emitter


	}
}