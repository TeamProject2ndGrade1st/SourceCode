#include "ArAudioManager.h"
#include "../Other/Misc.h"

namespace Argent::Resource::Audio
{
	ArAudioManager* ArAudioManager::instance = nullptr;
	ArAudioManager::ArAudioManager()
	{
		if (instance) _ASSERT_EXPR(FALSE, "already instanciated");
		instance = this;


		HRESULT hr{ S_OK };

		//�I�[�f�B�I�G���W���̍쐬
		hr = XAudio2Create(audioEngine.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		//�}�X�^�[�{�C�X�̍쐬
		hr = audioEngine->CreateMasteringVoice(&masterVoice);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
	}
}