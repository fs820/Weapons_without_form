//=============================================================================
//
// サウンド処理 [sound.cpp]
//
//=============================================================================
#include "sound.h"
#include "debug.h"

using namespace sound; // サウンドスペースの使用

//----------------------------------------------------
//
// サウンドクラス
//
//----------------------------------------------------

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT CSound::Play(const float volume)
{
	// 音楽が存在する
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// 状態の取得
	XAUDIO2_VOICE_STATE xa2state;
	m_pSourceVoice->GetState(&xa2state);

	// 既存の再生を停止
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

	// オーディオバッファの登録
	if (FAILED(m_pSourceVoice->SubmitSourceBuffer(m_pBuffer)))
	{
		return E_FAIL;
	}

	// 範囲制限
	m_baseVolume = clamp(volume, 0.0f, 1.0f);

	// 音量
	if (FAILED(m_pSourceVoice->SetVolume(m_baseVolume)))
	{
		return E_FAIL;
	}

	// 再生
	if (FAILED(m_pSourceVoice->Start(0)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(void)
{
	m_isFadeOut = true;             // フェードアウト開始
	m_fadeOutVolume = m_baseVolume; // フェードアウトボリューム
}

//=============================================================================
// 音量変更
//=============================================================================
HRESULT CSound::SetVolume(const float volume)
{
	// 音楽が存在する
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// 範囲制限
	m_baseVolume = clamp(volume, 0.0f, 1.0f);

	// 音量を変更
	return m_pSourceVoice->SetVolume(m_baseVolume);
}

//=============================================================================
// 音量変更
//=============================================================================
HRESULT CSound::AddVolume(const float volume)
{
	// 音楽が存在する
	if (m_pSourceVoice == nullptr)
	{
		return E_POINTER; // 失敗時のリターン
	}

	// 範囲制限
	m_baseVolume = clamp(m_baseVolume + volume, 0.0f, 1.0f);

	// 音量を変更
	return m_pSourceVoice->SetVolume(m_baseVolume);
}

//=============================================================================
// 更新
//=============================================================================
void CSound::Update(void)
{
	// 音楽が存在する
	if (m_pSourceVoice == nullptr)
	{
		return; // 失敗時のリターン
	}

	// フェードアウト
	// 状態の取得
	XAUDIO2_VOICE_STATE xa2state;
	m_pSourceVoice->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		if (m_isFadeOut)
		{// フェードアウト中
			// 音量が0になるまで音量を少しずつ下げる
			m_fadeOutVolume -= m_baseVolume * (CMain::GetDeltaTimeGameSpeed() / sound::FADE_TIME); // 少しずつ下げる
			m_fadeOutVolume = max(0.0f, m_fadeOutVolume);                                          // 下限値
			m_pSourceVoice->SetVolume(m_fadeOutVolume);                                            // 音量変更

			if (m_fadeOutVolume <= 0.0f)
			{// 音がなくなったら
				// 停止
				m_pSourceVoice->Stop(0);

				// 開放
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
// サウンド管理クラス
//
//----------------------------------------------------

// 静的メンバ変数
CSoundManager CSoundManager::m_instance{};     // 自分のインスタンス

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSoundManager::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		Uninit();
		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < Index8(LABEL::Max); nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aInfo[nCntSound].sFilename.data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);

			Uninit();
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);

			Uninit();
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_audioSource[nCntSound].sizeAudio, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}
		m_audioSource[nCntSound].pDataAudio = new BYTE[m_audioSource[nCntSound].sizeAudio];
		hr = ReadChunkData(hFile, m_audioSource[nCntSound].pDataAudio, m_audioSource[nCntSound].sizeAudio, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);

			Uninit();
			return S_FALSE;
		}

		// バッファの値設定
		memset(&m_audioSource[nCntSound].buffer, 0, sizeof(XAUDIO2_BUFFER));
		m_audioSource[nCntSound].buffer.AudioBytes = m_audioSource[nCntSound].sizeAudio;
		m_audioSource[nCntSound].buffer.pAudioData = m_audioSource[nCntSound].pDataAudio;
		m_audioSource[nCntSound].buffer.Flags = XAUDIO2_END_OF_STREAM;
		m_audioSource[nCntSound].buffer.LoopCount = m_aInfo[nCntSound].nCntLoop;
	
		// ソースボイスの生成
		for (size_t cntSound = 0; cntSound < sound::SOUND_MAX; cntSound++)
		{
			hr = m_pXAudio2->CreateSourceVoice(&m_audioSource[nCntSound].pSourceVoice[cntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);

				Uninit();
				return S_FALSE;
			}

			// オーディオバッファの登録
			m_audioSource[nCntSound].pSourceVoice[cntSound]->SubmitSourceBuffer(&m_audioSource[nCntSound].buffer);
		}

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSoundManager::Uninit(void)
{
	// 再生用クラス
	for (auto& label: m_apSound)
	{// サウンドループ
		SAFE_DELETE_ARRAY(label);
	}

	// ソースデータ
	for (auto& label : m_audioSource)
	{// ソースデータラベルループ
		// ソースボイスの破棄
		for (auto& source : label.pSourceVoice)
		{// 再生音声データループ
			if (source != nullptr)
			{// 存在する
				source->Stop(0);
				source->FlushSourceBuffers();
				source->DestroyVoice();
				source = nullptr;
			}
		}

		// オーディオデータの開放
		SAFE_DELETE(label.pDataAudio);
	}
	
	// マスターボイスの破棄
	if (m_pMasteringVoice != nullptr)
	{// 存在する
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}
	
	// XAudio2オブジェクトの開放
	SAFE_RELEASE(m_pXAudio2);
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// 更新
//=============================================================================
void CSoundManager::Update(void)
{
	for (auto& label : m_apSound)
	{// ラベル走査
		for (auto& Sound : label)
		{// データ走査
			if (Sound != nullptr)
			{// 存在する
				CDebugProc::Print(CDebugProc::MODE::System, "再生中:%d,%d", label);
				Sound->Update(); // 更新

				if (Sound->IsStop())
				{// 停止フラグ
					// 破棄
					delete Sound;
					Sound = nullptr;
					CDebugProc::Print(CDebugProc::MODE::System, "停止:%d,%d", label);
				}
			}
		}
	}
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
Index CSoundManager::Play(Index8 label, float volume)
{
	// `label` の範囲チェック
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
				CDebugProc::Print(CDebugProc::MODE::System, "停止:%d,%d", label, cntSound);
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
// セグメント停止(ラベル指定)
//=============================================================================
void CSoundManager::Stop(Index8 label, size_t id)
{
	// `label` の範囲チェック
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return;
	}
	// idの範囲チェック
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
// セグメント停止(全て)
//=============================================================================
void CSoundManager::Stop(void)
{
	// 一時停止
	for (auto& label : m_apSound)
	{// ラベル走査
		for (auto& Sound : label)
		{// データ走査
			if (Sound != nullptr)
			{
				Sound->Stop();
			}
		}
	}
}

//=============================================================================
// マスター音量変更
//=============================================================================
HRESULT CSoundManager::SetVolume(const float volume)
{
	// 範囲制限
	float clampVolume = clamp(volume, 0.0f, 1.0f);
	return m_pMasteringVoice->SetVolume(clampVolume);
}

//=============================================================================
// ソース音量変更
//=============================================================================
HRESULT CSoundManager::SetVolume(Index8 label, size_t id, const float volume)
{
	// `label` の範囲チェック
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return E_INVALIDARG;
	}
	// idの範囲チェック
	if (id < 0 || id >= sound::SOUND_MAX)
	{
		return E_INVALIDARG;
	}

	if (m_apSound[Index8(label)][id] == nullptr)
	{
		return E_POINTER;
	}

	// 音量を変更
	return m_apSound[Index8(label)][id]->SetVolume(volume);
}

//=============================================================================
// マスター音量変更
//=============================================================================
HRESULT CSoundManager::AddVolume(const float volume)
{
	float volumeOld{};
	m_pMasteringVoice->GetVolume(&volumeOld); // 今の音量
	volumeOld += volume;                      // 足す
	// 範囲制限
	volumeOld = clamp(volumeOld, 0.0f, 1.0f);
	return m_pMasteringVoice->SetVolume(volumeOld); // 変更
}

//=============================================================================
// ソース音量変更
//=============================================================================
HRESULT CSoundManager::AddVolume(Index8 label, size_t id, const float volume)
{
	// labelの範囲チェック
	if (label < 0 || label >= Index8(LABEL::Max))
	{
		return E_INVALIDARG;
	}
	// idの範囲チェック
	if (id < 0 || id >= sound::SOUND_MAX)
	{
		return E_INVALIDARG;
	}

	if (m_apSound[Index8(label)][id] == nullptr)
	{
		return E_POINTER;
	}

	// 音量を変更
	return m_apSound[Index8(label)][id]->AddVolume(volume);
}

//=============================================================================
// チャンクのチェック
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
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
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
// チャンクデータの読み込み
//=============================================================================
HRESULT CSoundManager::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}