// ReSharper disable CommentTypo
// ReSharper disable CppClangTidyModernizeUseNodiscard
// ReSharper restore CommentTypo
#pragma once
#include <xaudio2.h>
#include "ArResource.h"

namespace Argent::Resource::Audio
{
	/**
	 * \brief ���f�[�^�������Ă��@
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
		 * \brief �Đ��J�n�@���łɍĐ����������ꍇ�͏������X�L�b�v
		 */
		void Play();

		/**
		 * \brief ��~�@���łɒ�~�ς݂������ꍇ�̓X�L�b�v
		 */
		void Stop();
		/**
		 * \brief �ꎞ��~�@���łɈꎞ��~���������ꍇ�̓X�L�b�v�@
		 */
		void Pause();

		//�\�[�X�{�C�X�̏�Ԃ��擾�@�Đ���or��~��or�ꎞ��~��
		State GetState() const { return state; }
		bool IsPlaying() const { return state == State::Playing; }
		bool IsPausing() const { return state == State::Pausing; }
		bool IsStopping() const { return state == State::Stopping; }

	private:
		IXAudio2SourceVoice* sourceVoice;
		WAVEFORMATEXTENSIBLE wfx = {0};
		XAUDIO2_BUFFER buffer = {0};
		State state;

	private:
		/**
		 * \brief �܂��Đ��ł��Ă��Ȃ��o�b�t�@�̈悪���邩�m�F
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
