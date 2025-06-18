//-----------------------------------------
//
// サウンド処理 [sound.h]
// Author: null
//
//-----------------------------------------
#pragma once
#include "main.h"    // メイン
#include "xaudio2.h" // サウンド用

//---------------------------------------------------------
// サウンドクラス (渡されたサウンドの再生・停止などを行う)
//---------------------------------------------------------
class CSound final
{
	// 公開
public:
	CSound() : m_pSourceVoice{}, m_isFadeOut{}, m_isFadeOutEnd{}, m_pBuffer{}, m_baseVolume{}, m_fadeOutVolume{} {};
	CSound(IXAudio2SourceVoice* pSourceVoice, XAUDIO2_BUFFER* pBuffer) : m_pSourceVoice(pSourceVoice), m_isFadeOut{}, m_isFadeOutEnd{}, m_pBuffer(pBuffer), m_baseVolume{}, m_fadeOutVolume{} {};
	~CSound() = default;

	HRESULT Play(const float volume = 1.0f);
	void Stop(void);
	HRESULT SetVolume(const float volume);
	HRESULT AddVolume(const float volume);
	HRESULT GetVolume(float* pVolume) const { m_pSourceVoice->GetVolume(pVolume); };
	void Update(void);

	bool IsStop() { return m_isFadeOutEnd; };

	// 非公開
private:
	static constexpr float FADE_TIME = 1.0f;                        // フェードアウト時間
	static constexpr milliseconds FADE_INTERVAL = milliseconds(50); // フェードアウト間隔 ms

	IXAudio2SourceVoice* m_pSourceVoice;   // ソースボイス
	XAUDIO2_BUFFER* m_pBuffer;             // オーディオバッファーポインタ
	float m_baseVolume;                    // 基本ボリューム
	float m_fadeOutVolume;                 // フェードアウトボリューム
	bool m_isFadeOut;                      // フェードアウトフラグ
	bool m_isFadeOutEnd;                   // フェードアウト終了フラグ
};

//--------------------------------------------------------------------------------------------------------------
// サウンド管理クラス シングルトン コピー禁止 (サウンドデータを管理し、呼び出しに応じてCSoundに実行命令を行う)
//--------------------------------------------------------------------------------------------------------------
class CSoundManager final
{
// 公開
public:
    // サウンド列挙
	using LABEL = enum
	{
		LABEL_TEST_BGM,	// TestBgm
		LABEL_TEST_SE,	// TestSe
		LABEL_MAX       // サウンド数
	};

	static CSoundManager& GetInstance(void) { return m_instance; };

	CSoundManager(const CSoundManager&) = delete;
	CSoundManager& operator=(const CSoundManager&) = delete;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	size_t Play(LABEL label, float volume = 1.0f);
	void Stop(LABEL label, size_t id);
	void Stop(void);
	HRESULT SetVolume(const float volume);
	HRESULT SetVolume(LABEL label, size_t id, const float volume);
	HRESULT AddVolume(const float volume);
	HRESULT AddVolume(LABEL label, size_t id, const float volume);

// 非公開
private:
	static constexpr size_t SOUND_MAX = 16; // 1種類のサウンドの最大数

	// サウンド情報体定義
	using Info = struct
	{
		string_view sFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	};

	// オーディオソース構造体定義
	using AudioSource = struct
	{
		IXAudio2SourceVoice* pSourceVoice[SOUND_MAX]; // ソースボイス
		BYTE* pDataAudio;			                  // オーディオデータ
		DWORD sizeAudio;				              // オーディオデータサイズ
		XAUDIO2_BUFFER buffer;                        // オーディオバッファー
	};

	static CSoundManager m_instance;     // 自分のインスタンス

	CSoundManager() : m_pXAudio2{}, m_pMasteringVoice{}, m_apSound{},
		// サウンド情報
		m_aInfo{
	{"data/BGM/Test.wav", -1}, // TestBgm
	{"data/SE/Test.wav", 0}    // TestSe
	},
		m_audioSource{} {};
	~CSoundManager() = default;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;						// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;  // マスターボイス

	CSound* m_apSound[LABEL_MAX][SOUND_MAX];    // サウンドのインスタンス
	const Info m_aInfo[LABEL_MAX];              // サウンド情報
	AudioSource m_audioSource[LABEL_MAX];       // オーディオソース (再生用データ)
};