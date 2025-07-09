//-----------------------------------------
//
// サウンド処理 [sound.h]
// Author: null
//
//-----------------------------------------
#pragma once
#include "main.h"    // メイン
#include "xaudio2.h" // サウンド用

namespace sound
{
	// 同じソースを同時再生できる最大数
	constexpr Index8 SOUND_MAX = Index8(32);

	constexpr float FADE_TIME = 1.0f;                        // フェードアウト時間
	constexpr milliseconds FADE_INTERVAL = milliseconds(50); // フェードアウト間隔 ms

	// サウンド列挙
	enum class LABEL : Index8
	{
		TestBgm, // TestBgm
		TestSe,	 // TestSe
		Max      // サウンド数
	};

	// サウンド情報体定義
	struct Info
	{
		const string_view sFilename; // ファイル名
		const int nCntLoop;		     // ループカウント

		Info(string_view filename, int cntLoop) : sFilename(filename), nCntLoop(cntLoop) {}
		~Info() = default;
	};

	// オーディオソース構造体定義
	struct AudioSource
	{
		array<IXAudio2SourceVoice*, sound::SOUND_MAX> pSourceVoice; // ソースボイス
		BYTE* pDataAudio;			                                // オーディオデータ
		DWORD sizeAudio;				                            // オーディオデータサイズ
		XAUDIO2_BUFFER buffer;                                      // オーディオバッファー

		AudioSource() : pSourceVoice{}, pDataAudio{}, sizeAudio{}, buffer{} {}
		~AudioSource() = default;
	};
}

//---------------------------------------------------------
// サウンドクラス (渡されたサウンドの再生・停止などを行う)
//---------------------------------------------------------
class CSound final
{
	// 公開
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

	// 非公開
private:
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

	// 非公開
private:
	static CSoundManager m_instance;     // 自分のインスタンス

	CSoundManager() : m_pXAudio2{}, m_pMasteringVoice{}, m_apSound{},
		// サウンド情報
		m_aInfo{
			sound::Info{"data/BGM/Test.wav", -1}, // TestBgm
			sound::Info{"data/SE/Test.wav", 0}    // TestSe
	},
		m_audioSource{} {
	};
	~CSoundManager() = default;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2* m_pXAudio2;						// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;  // マスターボイス

	array<array<CSound*, sound::SOUND_MAX>, Index8(sound::LABEL::Max)> m_apSound; // サウンドインスタンスポインタ
	array<sound::Info, Index8(sound::LABEL::Max)> m_aInfo;                        // サウンド情報
	array<sound::AudioSource, Index8(sound::LABEL::Max)> m_audioSource;           // オーディオソース (再生用データ)
};