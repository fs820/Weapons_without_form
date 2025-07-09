//-----------------------------------------
//
// �T�E���h���� [sound.h]
// Author: null
//
//-----------------------------------------
#pragma once
#include "main.h"    // ���C��
#include "xaudio2.h" // �T�E���h�p

namespace sound
{
	// �����\�[�X�𓯎��Đ��ł���ő吔
	constexpr Index8 SOUND_MAX = Index8(32);

	constexpr float FADE_TIME = 1.0f;                        // �t�F�[�h�A�E�g����
	constexpr milliseconds FADE_INTERVAL = milliseconds(50); // �t�F�[�h�A�E�g�Ԋu ms

	// �T�E���h��
	enum class LABEL : Index8
	{
		TestBgm, // TestBgm
		TestSe,	 // TestSe
		Max      // �T�E���h��
	};

	// �T�E���h���̒�`
	struct Info
	{
		const string_view sFilename; // �t�@�C����
		const int nCntLoop;		     // ���[�v�J�E���g

		Info(string_view filename, int cntLoop) : sFilename(filename), nCntLoop(cntLoop) {}
		~Info() = default;
	};

	// �I�[�f�B�I�\�[�X�\���̒�`
	struct AudioSource
	{
		array<IXAudio2SourceVoice*, sound::SOUND_MAX> pSourceVoice; // �\�[�X�{�C�X
		BYTE* pDataAudio;			                                // �I�[�f�B�I�f�[�^
		DWORD sizeAudio;				                            // �I�[�f�B�I�f�[�^�T�C�Y
		XAUDIO2_BUFFER buffer;                                      // �I�[�f�B�I�o�b�t�@�[

		AudioSource() : pSourceVoice{}, pDataAudio{}, sizeAudio{}, buffer{} {}
		~AudioSource() = default;
	};
}

//---------------------------------------------------------
// �T�E���h�N���X (�n���ꂽ�T�E���h�̍Đ��E��~�Ȃǂ��s��)
//---------------------------------------------------------
class CSound final
{
	// ���J
public:
	CSound() : m_pSourceVoice{}, m_isFadeOut{}, m_isFadeOutEnd{}, m_pBuffer{}, m_baseVolume{}, m_fadeOutVolume{} {};
	CSound(IXAudio2SourceVoice* pSourceVoice, XAUDIO2_BUFFER* pBuffer) : m_pSourceVoice(pSourceVoice), m_isFadeOut{}, m_isFadeOutEnd{}, m_pBuffer(pBuffer), m_baseVolume{}, m_fadeOutVolume{} {}
	~CSound() = default;

	HRESULT Play(const float volume = 1.0f);
	void Stop(void);
	HRESULT SetVolume(const float volume);
	HRESULT AddVolume(const float volume);
	HRESULT GetVolume(float* pVolume) const { m_pSourceVoice->GetVolume(pVolume); }
	void Update(void);

	bool IsStop() { return m_isFadeOutEnd; }

	// ����J
private:
	IXAudio2SourceVoice* m_pSourceVoice;   // �\�[�X�{�C�X
	XAUDIO2_BUFFER* m_pBuffer;             // �I�[�f�B�I�o�b�t�@�[�|�C���^
	float m_baseVolume;                    // ��{�{�����[��
	float m_fadeOutVolume;                 // �t�F�[�h�A�E�g�{�����[��
	bool m_isFadeOut;                      // �t�F�[�h�A�E�g�t���O
	bool m_isFadeOutEnd;                   // �t�F�[�h�A�E�g�I���t���O
};

//--------------------------------------------------------------------------------------------------------------
// �T�E���h�Ǘ��N���X �V���O���g�� �R�s�[�֎~ (�T�E���h�f�[�^���Ǘ����A�Ăяo���ɉ�����CSound�Ɏ��s���߂��s��)
//--------------------------------------------------------------------------------------------------------------
class CSoundManager final
{
	// ���J
public:
	static CSoundManager& GetInstance(void) { return m_instance; }

	CSoundManager(const CSoundManager&) = delete;
	CSoundManager& operator=(const CSoundManager&) = delete;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	Index Play(Index8 label, float volume = 1.0f);
	void Stop(Index8 label, size_t id);
	void Stop(void);
	HRESULT SetVolume(const float volume);
	HRESULT SetVolume(Index8 label, size_t id, const float volume);
	HRESULT AddVolume(const float volume);
	HRESULT AddVolume(Index8 label, size_t id, const float volume);

	// ����J
private:
	static CSoundManager m_instance;     // �����̃C���X�^���X

	CSoundManager() : m_pXAudio2{}, m_pMasteringVoice{}, m_apSound{},
		// �T�E���h���
		m_aInfo{
			sound::Info{"data/BGM/Test.wav", -1}, // TestBgm
			sound::Info{"data/SE/Test.wav", 0}    // TestSe
	},
		m_audioSource{} {
	};
	~CSoundManager() = default;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;						// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice;  // �}�X�^�[�{�C�X

	array<array<CSound*, sound::SOUND_MAX>, Index8(sound::LABEL::Max)> m_apSound; // �T�E���h�C���X�^���X�|�C���^
	array<sound::Info, Index8(sound::LABEL::Max)> m_aInfo;                        // �T�E���h���
	array<sound::AudioSource, Index8(sound::LABEL::Max)> m_audioSource;           // �I�[�f�B�I�\�[�X (�Đ��p�f�[�^)
};