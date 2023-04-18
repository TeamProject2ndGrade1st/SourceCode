#include "AudioManager.h"
#include "../Other/Misc.h"
#include "../../../imgui/imgui.h"
#include "../Component/Transform.h"
#include "../Core/Timer.h"
#include "../Component/AudioPlayer.h"
#include "../Math/MathHelper.h"

#pragma comment(lib, "XAudio2.lib")

#include <xaudio2fx.h>
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


		XAUDIO2_VOICE_DETAILS details;
		masterVoice->GetVoiceDetails(&details);


		hr = XAudio2CreateReverb(&reverbEffect, 0);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));


		XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { reverbEffect, TRUE, 1 } };
		XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

		audioEngine->CreateSubmixVoice(&submixVoice, 1, details.InputSampleRate, 0, 0, nullptr,
			&effectChain);

		XAUDIO2FX_REVERB_I3DL2_PARAMETERS param = XAUDIO2FX_I3DL2_PRESET_FOREST;

		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative(&param, &native);
		submixVoice->SetEffectParameters(0, &native, sizeof(native));

		emitterPos = new Transform();
		listenerPos = new Transform();

		
		masterVoice->GetChannelMask(&dwChannelMask);

		hr = X3DAudioInitialize(dwChannelMask,
			X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		nChannel = details.InputChannels;
		for(int i = 0; i < 8; ++i)
		{
			outputMatrix[i] = 0;
		}
	}

	void AudioManager::Initialize()
	{
		audio = std::make_shared<Argent::Resource::Audio::AudioResource>("./Resources/Audio/maou.wav");
		audio->Play();
	}

	void AudioManager::Update()
	{
		static float deltaTime{};
		deltaTime += Argent::Timer::ArTimer::Instance().DeltaTime();
		

		audio->GetSourceVoice()->SetVolume(volume);

		DirectX::XMFLOAT3 rightVec = listenerPos->CalcRight();
		DirectX::XMFLOAT3 lis2EmitVec = emitterPos->GetPosition() - listenerPos->GetPosition();

		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(
			DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&rightVec)), 
			DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&lis2EmitVec)));
		DirectX::XMStoreFloat(&pan, Dot);

		float left = 0.5f - pan / 2;
		float right = 0.5f + pan / 2;
		//float matrix[16]{};

		switch (dwChannelMask)
		{
		case SPEAKER_MONO:
			outputMatrix[0] = 1.0;
			break;
		case SPEAKER_STEREO:
		case SPEAKER_2POINT1:
		case SPEAKER_SURROUND:
			outputMatrix[0] = left;
			outputMatrix[1] = right;
			break;
		case SPEAKER_QUAD:
			outputMatrix[0] = outputMatrix[2] = left;
			outputMatrix[1] = outputMatrix[3] = right;
			break;
		case SPEAKER_4POINT1:
			outputMatrix[0] = outputMatrix[3] = left;
			outputMatrix[1] = outputMatrix[4] = right;
			break;
		case SPEAKER_5POINT1:
		case SPEAKER_7POINT1:
		case SPEAKER_5POINT1_SURROUND:
			outputMatrix[0] = outputMatrix[4] = left;
			outputMatrix[1] = outputMatrix[5] = right;
			break;
		case SPEAKER_7POINT1_SURROUND:
			outputMatrix[0]  = outputMatrix[1] = outputMatrix[8] = 
				outputMatrix[9] = outputMatrix[12] = outputMatrix[13] = left;
			outputMatrix[2]  = outputMatrix[3] = outputMatrix[10] = 
				outputMatrix[11] = outputMatrix[14] = outputMatrix[15] = right;
			break;
		}
		if (deltaTime > 1.0f / 144.0f)
		{
			static DirectX::XMFLOAT3 emitterPost = DirectX::XMFLOAT3();
			static DirectX::XMFLOAT3 listenerPost = DirectX::XMFLOAT3();
			X3DAUDIO_LISTENER listener = {};
			X3DAUDIO_EMITTER emitter = {};
			emitter.ChannelCount = 2;
			emitter.CurveDistanceScaler = emitter.DopplerScaler = 1.0f;
			emitter.ChannelRadius = 1;
			emitter.pChannelAzimuths = new FLOAT32;

			//これは何してるかよくわからない
			X3DAUDIO_DSP_SETTINGS DSPSettings = {};
			DSPSettings.SrcChannelCount = 2;
			DSPSettings.DstChannelCount = 8;
			DSPSettings.pDelayTimes = nullptr;
			XAUDIO2_VOICE_DETAILS voiceDetail{};
			masterVoice->GetVoiceDetails(&voiceDetail);
			DSPSettings.pMatrixCoefficients = new FLOAT32[2 * 8];


			//3dオーディオ設定を計算
			//リスナーとエミッタの座標等々を設定
			emitter.OrientFront = emitterPos->CalcForward();
			emitter.OrientTop = emitterPos->CalcUp();
			emitter.Position = emitterPos->GetPosition();
			emitter.Velocity = emitterPos->GetPosition() - emitterPost;

			listener.OrientFront = listenerPos->CalcForward();
			listener.OrientTop = listenerPos->CalcUp();
			listener.Position = listenerPos->GetPosition();
			listener.Velocity = listenerPos->GetPosition() - listenerPost;


			DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
				| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
				| X3DAUDIO_CALCULATE_REVERB;
			//計算
			X3DAudioCalculate(X3DInstance, &listener, &emitter, 
				dwCalcFlags,
				&DSPSettings);


			auto sourceVoice = audio->GetSourceVoice();

			sourceVoice->SetOutputMatrix(masterVoice, 2, nChannel,
				DSPSettings.pMatrixCoefficients);

	//		sourceVoice->SetOutputMatrix(submixVoice, 1, 1, &DSPSettings.ReverbLevel);
	//		XAUDIO2_FILTER_PARAMETERS filterParameters = { LowPassFilter,
	//			2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
	//		sourceVoice->SetOutputFilterParameters(masterVoice, &filterParameters);

	//		XAUDIO2_FILTER_PARAMETERS filterParametersReberb = { LowPassFilter,
	//2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
	//		sourceVoice->SetOutputFilterParameters(submixVoice, &filterParametersReberb);


	//		emitterPost = emitterPos->GetPosition();
	//		listenerPost = listenerPos->GetPosition();

			

			deltaTime = 0;
		}

		//XAUDIO2_VOICE_DETAILS voiceDetails;
		//audio->GetSourceVoice()->GetVoiceDetails(&voiceDetails);
		//XAUDIO2_VOICE_DETAILS masterVoiceDetails;
		//masterVoice->GetVoiceDetails(&masterVoiceDetails);
		//audio->GetSourceVoice()->SetOutputMatrix(nullptr, voiceDetails.InputChannels, masterVoiceDetails.InputChannels, outputMatrix);
		//volume = 2.f - fabsf(pan) * 1.1f;
	}

	void AudioManager::DrawDebug()
	{
		if (ImGui::TreeNode("AudioManager"))
		{
			ImGui::SliderFloat("Pan", &pan, -1.0f, 1.0f);
			for(int i = 0;  i < 16; ++i)
			{
				ImGui::SliderFloat(std::to_string(i).c_str(), &outputMatrix[i], 0, 1.0f);
			}
			static bool b = false;
			ImGui::Checkbox("centerValue", &b);
			if(b)
			{
				outputMatrix[4] =
					outputMatrix[5] =
					outputMatrix[6] =
					outputMatrix[7] = 1.0f - fabsf(pan);
			}
			else
			{
				outputMatrix[4] =
					outputMatrix[5] =
					outputMatrix[6] =
					outputMatrix[7] = 1.0f;
			}
			ImGui::SliderFloat("Volum", &volume, 0.0f, 10.0f);
			if(emitterPos)
			{
				if (ImGui::TreeNode("Emitter"))
				{
					emitterPos->DrawDebug();
					ImGui::TreePop();
				}
			}
			if(listenerPos)
			{
				if(ImGui::TreeNode("Listener"))
				{
					listenerPos->DrawDebug();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}
