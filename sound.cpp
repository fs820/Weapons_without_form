//=============================================================================
//
// �T�E���h���� [sound.cpp]
//
//=============================================================================
#include "sound.h"
#include "debug.h"

using namespace sound; // �T�E���h�X�y�[�X�̎g�p

//----------------------------------------------------
//
// �T�E���h�N���X
//
//----------------------------------------------------

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT CSound::Play(const float volume)
{
	// ���y�����݂���
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// ��Ԃ̎擾
	XAUDIO2_VOICE_STATE xa2state;
	m_pSourceVoice->GetState(&xa2state);

	// �����̍Đ����~
	if (xa2state.BuffersQueued > 0)
	{
		if (FAILED(m_pSourceVoice->Stop(0)))
		{
			return E_FAIL;
		}
		if (FAILED(m_pSourceVoice->FlushSourceBuffers()))
		{
			return E_FAIL;
		}
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	if (FAILED(m_pSourceVoice->SubmitSourceBuffer(m_pBuffer)))
	{
		return E_FAIL;
	}

	// �͈͐���
	m_baseVolume = clamp(volume, 0.0f, 1.0f);

	// ����
	if (FAILED(m_pSourceVoice->SetVolume(m_baseVolume)))
	{
		return E_FAIL;
	}

	// �Đ�
	if (FAILED(m_pSourceVoice->Start(0)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~
//=============================================================================
void CSound::Stop(void)
{
	m_isFadeOut = true;             // �t�F�[�h�A�E�g�J�n
	m_fadeOutVolume = m_baseVolume; // �t�F�[�h�A�E�g�{�����[��
}

//=============================================================================
// ���ʕύX
//=============================================================================
HRESULT CSound::SetVolume(const float volume)
{
	// ���y�����݂���
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// �͈͐���
	m_baseVolume = clamp(volume, 0.0f, 1.0f);

	// ���ʂ�ύX
	return m_pSourceVoice->SetVolume(m_baseVolume);
}

//=============================================================================
// ���ʕύX
//=============================================================================
HRESULT CSound::AddVolume(const float volume)
{
	// ���y�����݂���
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // ���s���̃��^�[��
	}

	// �͈͐���
	m_baseVolume = clamp(m_baseVolume + volume, 0.0f, 1.0f);

	// ���ʂ�ύX
	return m_pSourceVoice->SetVolume(m_baseVolume);
}

//=============================================================================
// �X�V
//=============================================================================
void CSound::Update(void)
{
	// ���y�����݂���
	if (m_pSourceVoice == nullptr)
	{
		return; // ���s���̃��^�[��
	}

	// �t�F�[�h�A�E�g
	// ��Ԃ̎擾
	XAUDIO2_VOICE_STATE xa2state;
	m_pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		if (m_isFadeOut)
		{// �t�F�[�h�A�E�g��
			// ���ʂ�0�ɂȂ�܂ŉ��ʂ�������������
			m_fadeOutVolume -= m_baseVolume * (CMain::GetDeltaTimeGameSpeed() / sound::FADE_TIME); // ������������
			m_fadeOutVolume = max(0.0f, m_fadeOutVolume);                                          // �����l
			m_pSourceVoice->SetVolume(m_fadeOutVolume);                                            // ���ʕύX

			if (m_fadeOutVolume <= 0.0f)
			{// �����Ȃ��Ȃ�����
				// ��~
				m_pSourceVoice->Stop(0);

				// �J��
				m_pSourceVoice->FlushSourceBuffers();

				m_isFadeOutEnd = true;
			}
		}
	}
	else
	{
		m_isFadeOutEnd = true;
	}
}

//----------------------------------------------------
//
// �T�E���h�Ǘ��N���X
//
//----------------------------------------------------

// �ÓI�����o�ϐ�
CSoundManager CSoundManager::m_instance{};     // �����̃C���X�^���X

//=============================================================================
// ����������
//=============================================================================
HRESULT CSoundManager::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COM���C�u�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < Index8(LABEL::Max); nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aInfo[nCntSound].sFilename.data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			Uninit();
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			Uninit();
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_audioSource[nCntSound].sizeAudio, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		m_audioSource[nCntSound].pDataAudio = new BYTE[m_audioSource[nCntSound].sizeAudio];
		hr = ReadChunkData(hFile, m_audioSource[nCntSound].pDataAudio, m_audioSource[nCntSound].sizeAudio, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&m_audioSource[nCntSound].buffer, 0, sizeof(XAUDIO2_BUFFER));
		m_audioSource[nCntSound].buffer.AudioBytes = m_audioSource[nCntSound].sizeAudio;
		m_audioSource[nCntSound].buffer.pAudioData = m_audioSource[nCntSound].pDataAudio;
		m_audioSource[nCntSound].buffer.Flags = XAUDIO2_END_OF_STREAM;
		m_audioSource[nCntSound].buffer.LoopCount = m_aInfo[nCntSound].nCntLoop;
	
		// �\�[�X�{�C�X�̐���
		for (size_t cntSound = 0; cntSound < sound::SOUND_MAX; cntSound++)
		{
			hr = m_pXAudio2->CreateSourceVoice(&m_audioSource[nCntSound].pSourceVoice[cntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

				Uninit();
				return S_FALSE;
			}

			// �I�[�f�B�I�o�b�t�@�̓o�^
			m_audioSource[nCntSound].pSourceVoice[cntSound]->SubmitSourceBuffer(&m_audioSource[nCntSound].buffer);
		}

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSoundManager::Uninit(void)
{
	// �Đ��p�N���X
	for (auto& label: m_apSound)
	{// �T�E���h���[�v
		SAFE_DELETE_ARRAY(label);
	}

	// �\�[�X�f�[�^
	for (auto& label : m_audioSource)
	{// �\�[�X�f�[�^���x�����[�v
		// �\�[�X�{�C�X�̔j��
		for (auto& source : label.pSourceVoice)
		{// �Đ������f�[�^���[�v
			if (source != nullptr)
			{// ���݂���
				source->Stop(0);
				source->FlushSourceBuffers();
				source->DestroyVoice();
				source = nullptr;
			}
		}

		// �I�[�f�B�I�f�[�^�̊J��
		SAFE_DELETE(label.pDataAudio);
	}
	
	// �}�X�^�[�{�C�X�̔j��
	if (m_pMasteringVoice != nullptr)
	{// ���݂���
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}
	
	// XAudio2�I�u�W�F�N�g�̊J��
	SAFE_RELEASE(m_pXAudio2);
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �X�V
//=============================================================================
void CSoundManager::Update(void)
{
	for (auto& label : m_apSound)
	{// ���x������
		for (auto& Sound : label)
		{// �f�[�^����
			if (Sound != nullptr)
			{// ���݂���
				CDebugProc::Print(CDebugProc::MODE::System, "�Đ���:%d,%d", label);
				Sound->Update(); // �X�V

				if (Sound->IsStop())
				{// ��~�t���O
					// �j��
					delete Sound;
					Sound = nullptr;
					CDebugProc::Print(CDebugProc::MODE::System, "��~:%d,%d", label);
				}
			}
		}
	}
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
Index CSoundManager::Play(Index8 label, float volume)
{
	// `label` �͈̔̓`�F�b�N
	if (label < 0 || label >= Index(LABEL::Max))
	{
		return INVALID_ID;
	}

	for (Index cntSound = 0; cntSound < sound::SOUND_MAX; ++cntSound)
	{
		if (m_audioSource[label].pSourceVoice[cntSound] == nullptr)
		{
			m_apSound[label][cntSound] = new CSound(m_audioSource[label].pSourceVoice[cntSound], &m_audioSource[label].buffer);

			if (m_apSound[label][cntSound] != nullptr)
			{
				if (FAILED(m_apSound[label][cntSound]->Play(volume)))return INVALID_ID;
				CDebugProc::Print(CDebugProc::MODE::System, "��~:%d,%d", label, cntSound);
				return cntSound;
			}
			else
			{
				return INVALID_ID;
			}
		}
	}
	return INVALID_ID;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void CSoundManager::Stop(Index8 label, size_t id)
{
	// `label` �͈̔̓`�F�b�N
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return;
	}
	// id�͈̔̓`�F�b�N
	if (id < 0 || id >= sound::SOUND_MAX)
	{
		return;
	}

	if (m_apSound[Index8(label)][id] != nullptr)
	{
		m_apSound[Index8(label)][id]->Stop();
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void CSoundManager::Stop(void)
{
	// �ꎞ��~
	for (auto& label : m_apSound)
	{// ���x������
		for (auto& Sound : label)
		{// �f�[�^����
			if (Sound != nullptr)
			{
				Sound->Stop();
			}
		}
	}
}

//=============================================================================
// �}�X�^�[���ʕύX
//=============================================================================
HRESULT CSoundManager::SetVolume(const float volume)
{
	// �͈͐���
	float clampVolume = clamp(volume, 0.0f, 1.0f);
	return m_pMasteringVoice->SetVolume(clampVolume);
}

//=============================================================================
// �\�[�X���ʕύX
//=============================================================================
HRESULT CSoundManager::SetVolume(Index8 label, size_t id, const float volume)
{
	// `label` �͈̔̓`�F�b�N
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return E_INVALIDARG;
	}
	// id�͈̔̓`�F�b�N
	if (id < 0 || id >= sound::SOUND_MAX)
	{
		return E_INVALIDARG;
	}

	if (m_apSound[Index8(label)][id] == nullptr)
	{
		return E_POINTER;
	}

	// ���ʂ�ύX
	return m_apSound[Index8(label)][id]->SetVolume(volume);
}

//=============================================================================
// �}�X�^�[���ʕύX
//=============================================================================
HRESULT CSoundManager::AddVolume(const float volume)
{
	float volumeOld{};
	m_pMasteringVoice->GetVolume(&volumeOld); // ���̉���
	volumeOld += volume;                      // ����
	// �͈͐���
	volumeOld = clamp(volumeOld, 0.0f, 1.0f);
	return m_pMasteringVoice->SetVolume(volumeOld); // �ύX
}

//=============================================================================
// �\�[�X���ʕύX
//=============================================================================
HRESULT CSoundManager::AddVolume(Index8 label, size_t id, const float volume)
{
	// label�͈̔̓`�F�b�N
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return E_INVALIDARG;
	}
	// id�͈̔̓`�F�b�N
	if (id < 0 || id >= sound::SOUND_MAX)
	{
		return E_INVALIDARG;
	}

	if (m_apSound[Index8(label)][id] == nullptr)
	{
		return E_POINTER;
	}

	// ���ʂ�ύX
	return m_apSound[Index8(label)][id]->AddVolume(volume);
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSoundManager::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSoundManager::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}