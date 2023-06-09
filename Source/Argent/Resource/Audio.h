// ReSharper disable CommentTypo
// ReSharper disable CppClangTidyModernizeUseNodiscard
// ReSharper restore CommentTypo
#pragma once
#include <xaudio2.h>
#include "ArResource.h"

namespace Argent::Resource::Audio
{
	/**
	 * \brief 音データを持ってるやつ　
	 */
	class AudioResource:
		public ArResource
	{
		enum class State
		{
			Playing,
			Stopping,
			Pausing
		};

	public:
		AudioResource(const char* filePath);

		/**
		 * \brief 再生開始　すでに再生中だった場合は処理をスキップ
		 */
		void Play();

		/**
		 * \brief 停止　すでに停止済みだった場合はスキップ
		 */
		void Stop();
		/**
		 * \brief 一時停止　すでに一時停止中だった場合はスキップ　
		 */
		void Pause();

		//ソースボイスの状態を取得　再生中or停止中or一時停止中
		State GetState() const { return state; }
		bool IsPlaying() const { return state == State::Playing; }
		bool IsPausing() const { return state == State::Pausing; }
		bool IsStopping() const { return state == State::Stopping; }

		IXAudio2SourceVoice* GetSourceVoice() const { return sourceVoice;  }
	private:
		IXAudio2SourceVoice* sourceVoice;
		WAVEFORMATEXTENSIBLE wfx = {0};
		XAUDIO2_BUFFER buffer = {0};
		State state;

	private:
		/**
		 * \brief まだ再生できていないバッファ領域があるか確認
		 * \return 
		 */
		bool IsLastAudioQueue() const  // NOLINT(modernize-use-nodiscard)
		{
			XAUDIO2_VOICE_STATE voiceState = {};
			sourceVoice->GetState(&voiceState);
			return voiceState.BuffersQueued != 0;
		}
	};
}
