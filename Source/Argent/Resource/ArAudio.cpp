#include "ArAudio.h"
#include "../Other/Misc.h"
#include <filesystem>
#include "ArAudioManager.h"


namespace Argent::Resource::Audio
{
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


	//�`�����N�̓ǂݍ��� 
	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & chunkSize, DWORD & chunkDataPosition)
	{
	    HRESULT hr = S_OK;
	    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
	        return HRESULT_FROM_WIN32( GetLastError() );

	    DWORD chunkType;
	    DWORD chunkDataSize;
	    DWORD RIFFDataSize = 0;
	    DWORD fileType;
	    DWORD bytesRead = 0;
	    DWORD offset = 0;

	    while (hr == S_OK)
	    {
	        DWORD numBytesRead;

			//ReadFile�͓ǂނ��тɃ|�C���^�̈ʒu���ς��H�@�ǂ񂾕����������H�ۂ�


	    	//chunkType �� chunkDataSize�ɏ�������
	        if( 0 == ReadFile( hFile, &chunkType, sizeof(DWORD), &numBytesRead, nullptr ) )
	            hr = HRESULT_FROM_WIN32( GetLastError() );

	        if( 0 == ReadFile( hFile, &chunkDataSize, sizeof(DWORD), &numBytesRead, nullptr ) )
	            hr = HRESULT_FROM_WIN32( GetLastError() );

	        switch (chunkType)
	        {
	        case fourccRIFF:
	            RIFFDataSize = chunkDataSize;
	            chunkDataSize = 4; // sizeof(DWORD)��4������H
	            if( 0 == ReadFile( hFile, &fileType, sizeof(DWORD), &numBytesRead, nullptr ) )
	                hr = HRESULT_FROM_WIN32( GetLastError() );
	            break;

	        default:
	            if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, static_cast<LONG>(chunkDataSize), nullptr, FILE_CURRENT ) )
	            return HRESULT_FROM_WIN32( GetLastError() );            
	        }

			//�t�@�C����ǂ񂾕��������炷
	        offset += sizeof(DWORD) * 2;

	        if (chunkType == fourcc)
	        {
	            chunkSize = chunkDataSize;
	            chunkDataPosition = offset;
	            return S_OK;
	        }

			//�`�����N�^�C�v���Ⴄ�ꍇ�݂̂��炷
	        offset += chunkDataSize;

	        if (bytesRead >= RIFFDataSize) return S_FALSE;

	    }
	    return S_OK;
	}

	HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD bufferSize, DWORD bufferOffset)
	{
	    HRESULT hr = S_OK;
	    if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, static_cast<LONG>(bufferOffset), nullptr, FILE_BEGIN ) )
	        return HRESULT_FROM_WIN32( GetLastError() );
	    DWORD numBytesRead;
	    if( 0 == ReadFile( hFile, buffer, bufferSize, &numBytesRead, nullptr ) )
	        hr = HRESULT_FROM_WIN32( GetLastError() );
	    return hr;
	}

	ArAudio::ArAudio(const char* filePath):
		state(State::Stopping)
	{
		HRESULT hr{ S_OK };

		std::filesystem::path path{ filePath };
		//�t�@�C���I�[�v��
		HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		}

		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		}

		DWORD chunkSize;
		DWORD chunkPosition;
		//'WAVE' �� 'XWMA'�@�̂�OK
		FindChunk(hFile, fourccRIFF, chunkSize, chunkPosition);
		DWORD fileType;
		ReadChunkData(hFile, &fileType, sizeof(DWORD), chunkPosition);
		_ASSERT_EXPR(fileType == fourccWAVE, L"Only support 'WAVE'");


		//fourccFMT�̃`�����N�ɂ���f�[�^�� WAVEFORMATEXTENSIBLE(wfx)�ɃR�s�[����.
		FindChunk(hFile, fourccFMT, chunkSize, chunkPosition);
		ReadChunkData(hFile, &wfx, chunkSize, chunkPosition);

		//fourccDATA�̃`�����N�ɂ���f�[�^���R�s�[����
		FindChunk(hFile, fourccDATA, chunkSize, chunkPosition);
		BYTE* data = new BYTE[chunkSize];
		ReadChunkData(hFile, data, chunkSize, chunkPosition);

		buffer.AudioBytes = chunkSize;  //�o�b�t�@�̃T�C�Y
		buffer.pAudioData = data;  //�o�b�t�@�̒��g
		buffer.Flags = XAUDIO2_END_OF_STREAM; //�o�b�t�@�̌��ɂ͂����f�[�^���Ȃ����Ƃ𖾎�

		//�\�[�X�{�C�X�����
		hr = ArAudioManager::Instance().GetAudioEngine()->CreateSourceVoice(&sourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx));
		//hr = ArAudioManager::Instance()->GetAudioEngine()->CreateSourceVoice(&sourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx));
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));


		XAUDIO2_VOICE_STATE voice_state = {};
		sourceVoice->GetState(&voice_state);

		if (voice_state.BuffersQueued)
		{
			return;
		}
	}

	void ArAudio::Play()
	{
		if(IsPlaying()) return;

		HRESULT hr{ S_OK };
		/// <summary>
		/// ��~�����ꍇ�͂������submit���Ȃ��ƍĐ��ł��Ȃ�
		/// </summary>
		if(state == State::Stopping)
		{
			buffer.LoopCount = 10;
			hr = sourceVoice->SubmitSourceBuffer(&buffer);
			_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		}

		hr = sourceVoice->Start(0);
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		state = State::Playing;
	}

	void ArAudio::Stop()
	{
		if(!IsLastAudioQueue()) return;

		HRESULT hr{};
		hr = sourceVoice->Stop();
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		hr = sourceVoice->FlushSourceBuffers();
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

		state = State::Stopping;
	}

	void ArAudio::Pause()
	{
		if(!IsLastAudioQueue()) return;

		HRESULT hr{};
		hr = sourceVoice->Stop();
		_ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));
		state = State::Pausing;
	}
}
