//------------------------------------------
//
// マネージャー [manager.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "file.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "math.h"
#include "enemy.h"
#include "effect.h"
#include "debug.h"
#include "sound.h"
#include "number.h"
#include "score.h"
#include "time.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "objectbillboard.h"
#include "model.h"
#include "objectX.h"
#include "gui.h"

//------------------------------
//
// マネージャークラス
//
//------------------------------

// 静的メンバ変数の定義
const path CManager::m_textureFile = "data/TXT/texture.txt"; // テクスチャリスト
// 使いテクスチャを指定 (txtのタグを基にする)
const string_view CManager::m_textureTagList[] =
{
	"Number",
	"Enemy1","Enemy2",
	"Bullet1","Bullet2",
	"EX1","EX2",
	"Shadow_Effect"
};

CRenderer* CManager::m_pRenderer{};
CFile* CManager::m_pFile{};
CCamera* CManager::m_pCamera{};
CLight* CManager::m_pLight{};
CObject3D* CManager::m_p3DObject{};
CPlayer* CManager::m_pPlayer{};
CTime* CManager::m_pTime{};
CScore* CManager::m_pScore{};
CGui* CManager::m_pGui{};

// ポーズ状態かどうか
bool CManager::m_bPause{};

//---------------
// 初期化
//---------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 描画クラス
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}

	if (m_pRenderer != nullptr)
	{
		if (FAILED(m_pRenderer->Init(hInstance, hWnd, bWindow)))
		{
			return E_FAIL;
		}
	}

	// ファイル
	if (m_pFile == nullptr)
	{
		m_pFile = new CFile;
	}

	// カメラ
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}

	// ライト
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
	}

	if (m_pLight != nullptr)
	{
		m_pLight->Init();
	}

	// Gui
	if (m_pGui == nullptr)
	{
		m_pGui = new CGui;
	}

	if (m_pGui != nullptr)
	{
		m_pGui->Init();
	}

	//// インプット
	//if (FAILED(CInput::GetInstance().Init(hInstance, hWnd)))return E_FAIL;

	// サウンド
	if (FAILED(CSoundManager::GetInstance().Init(CMain::GethWnd())))return E_FAIL;

	// デバック表示
	if (FAILED(CDebugProc::Init()))return E_FAIL;

	// 読み込み
	
	//-------------------------------
	// 規定テクスチャのパスを取得する
	//-------------------------------
	vector<path> sTextureList{};
	vector<D3DXVECTOR2> textureBlockList{};
	m_pFile->ChangeFile(m_textureFile);    // テクスチャリストファイルをセット
	string readData = m_pFile->ReadText(); // テクスチャファイルの文字列を取得
	for (const auto& tag : m_textureTagList)
	{// 読み込見たいフラグ
		// フラグを基にパスとブロック数を取得する
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			sTextureList.push_back(sPath.value());
		}

		auto blockX = StringToData<float>(readData, tag, 1);
		auto blockY = StringToData<float>(readData, tag, 2);
		if (blockX.has_value() && blockY.has_value())
		{
			D3DXVECTOR2 textureBlock{ blockX.value(),blockY.value() };
			textureBlockList.push_back(textureBlock);
		}
	}
	if (FAILED(CTextureManager::GetInstance().Load(sTextureList, textureBlockList)))return E_FAIL; // 規定テクスチャを読み込む

	if (FAILED(CModelManager::GetInstance().Load()))return E_FAIL;   // 規定モデル

	// ゲーム
	if (FAILED(GameLoad()))return E_FAIL;  // ロード
	if (FAILED(GameStart()))return E_FAIL; // スタート

	return S_OK;
}

//---------------
// 終了
//---------------
void CManager::Uninit(void)
{
	GameEnd();    // ゲームの終了
	GameUnload(); // ソースの破棄

	// デバック表示
	CDebugProc::Uninit();

	// サウンド
	CSoundManager::GetInstance().Uninit();

	// モデル
	CModelManager::GetInstance().Unload();

	// テクスチャ
	CTextureManager::GetInstance().Unload();

	//// インプット
	//CInput::GetInstance().Uninit();

	// Gui
	if (m_pGui != nullptr)
	{
		m_pGui->Uninit(); //終了処理
		delete m_pGui;    // インスタンス破棄
		m_pGui = nullptr; // nullptr
	}

	// ライト
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit(); //終了処理
		delete m_pLight;    // インスタンス破棄
		m_pLight = nullptr;    // nullptr
	}

	// カメラ
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit(); //終了処理
		delete m_pCamera;    // インスタンス破棄
		m_pCamera = nullptr; // nullptr
	}

	// ファイル
	if (m_pFile != nullptr)
	{
		delete m_pFile;    // インスタンス破棄
		m_pFile = nullptr; // nullptr
	}

	// レンダラー
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit(); //終了処理
		delete m_pRenderer;    // インスタンス破棄
		m_pRenderer = nullptr;    // nullptr
	}
}

//---------------
// 更新
//---------------
void CManager::Update(void)
{
	//// インプット更新
	//CInput::GetInstance().Update();

	//-----------------------------
    // 管理&Debug操作関係
    //-----------------------------
	if (m_pTime->GetTime() <= 0.0f)
	{// 時間がなくなったら
		GameEnd();   // 終わらせる
		GameStart(); // 改めてスタート処理
	}

	// カーソル位置
	D3DXVECTOR2 cursorPos{};
	m_pRenderer->GetCursorPos(&cursorPos);
	CDebugProc::Print("Cursor位置 X:%.2f Y:%.2f", cursorPos.x, cursorPos.y);

	//CDebugProc::Print("ポーズ:[P]");
	//if (m_pInputKeyboard->GetTrigger(DIK_P))
	//{// ポーズ切替
	//	m_bPause = !m_bPause;
	//}

	//CDebugProc::Print("デバック表示切替:[F2]");
	//if (m_pInputKeyboard->GetTrigger(DIK_F2))
	//{// デバック表示切替
	//	CDebugProc::ToggleDebugDraw();
	//}
	//CDebugProc::Print("デバックモード切替:[F3]");
	//if (m_pInputKeyboard->GetTrigger(DIK_F3))
	//{// デバックモード切替
	//	CDebugProc::ChangeDrawMode();
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "強制スコア増減:[1][2]");
	//if (m_pInputKeyboard->GetPress(DIK_1))
	//{// スコア
	//	m_pScore->AddScore(1);
	//}
	//if (m_pInputKeyboard->GetPress(DIK_2))
	//{// スコア
	//	m_pScore->AddScore(-1);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "強制BGM:[B][N]");
	//if (m_pInputKeyboard->GetTrigger(DIK_B))
	//{// サウンド
	//	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_BGM);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_N))
	//{// サウンド
	//	CSoundManager::GetInstance().Stop(CSoundManager::LABEL_TEST_BGM, 0);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "BGM音量:[J][K]");
	//if (m_pInputKeyboard->GetTrigger(DIK_J))
	//{// サウンド
	//	CSoundManager::GetInstance().AddVolume(CSoundManager::LABEL_TEST_BGM, 0, 0.1f);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_K))
	//{// サウンド
	//	CSoundManager::GetInstance().AddVolume(CSoundManager::LABEL_TEST_BGM, 0, -0.1f);
	//}

	//CDebugProc::Print(CDebugProc::SYSTEM, "強制SE:[G][H]");
	//if (m_pInputKeyboard->GetTrigger(DIK_G))
	//{// サウンド
	//	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_SE);
	//}
	//if (m_pInputKeyboard->GetTrigger(DIK_H))
	//{// サウンド
	//	CSoundManager::GetInstance().Stop(CSoundManager::LABEL_TEST_SE, 0);
	//}

	//-----------------------------
    // 管理&Debug操作関係 End
    //-----------------------------

	m_pCamera->Update();

	// レンダラーの更新
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// サウンド更新
	CSoundManager::GetInstance().Update();
}

//---------------
// 描画
//---------------
void CManager::Draw(void)
{
	// レンダラーの描画
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}
}

//---------------------------------
// ゲームのロード処理
//---------------------------------
HRESULT CManager::GameLoad(void)
{
	//--------------------
	// ソース読み込み
    //--------------------

	const string_view sModelList[] =
	{
		"data\\MODEL\\Playermodel\\00_Player_Body.x",
		"data\\MODEL\\Playermodel\\01_Player_Head.x",
		"data\\MODEL\\Playermodel\\02_Player_RightArm.x",
		"data\\MODEL\\Playermodel\\03_Player_RightHand.x",
		"data\\MODEL\\Playermodel\\04_Player_LeftArm.x",
		"data\\MODEL\\Playermodel\\05_Player_LeftHand.x",
		"data\\MODEL\\Playermodel\\06_Player_RightLeg.x",
		"data\\MODEL\\Playermodel\\07_Player_RightFoot.x",
		"data\\MODEL\\Playermodel\\08_Player_LeftLeg.x",
		"data\\MODEL\\Playermodel\\09_Player_LeftFoot.x",
		"data\\MODEL\\Playermodel\\10_Player_Hat.x"
	};
	const D3DXVECTOR3 sModelOffSet[] =
	{
		{ 0.0f,20.0f,0.0f },
		{ 0.0f,25.0f,0.0f },
		{ -7.0f,20.0f,0.0f },
		{ -7.0f,0.0f,0.0f },
		{ 7.0f,20.0f,0.0f },
		{ 7.0f,0.0f,0.0f },
		{ -5.0f,0.0f,0.0f },
		{ 0.0f,-7.0f,0.0f },
		{ 5.0f,0.0f,0.0f },
		{ 0.0f,-7.0f,0.0f },
		{ 0.0f,15.0f,0.0f }
	};
	const int sModelParents[] =
	{
		-1,
		0,
		0,
		2,
		0,
		4,
		0,
		6,
		0,
        8,
		1
	};
	if (FAILED(CPlayer::Load(11, sModelList, sModelOffSet, sModelParents))) return E_FAIL;     // プレイヤー

	return S_OK;
}

//---------------------------------
// ゲームのスタート処理
//---------------------------------
HRESULT CManager::GameStart(void)
{
	//--------------------
	// インスタンス生成
	//--------------------

	// プレイヤー
	float scale = CMath::Random(1.0f, 1.0f);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, RandomAngle, 0.0f), D3DXVECTOR3(scale, scale, scale), CMath::Random(100.0f, 500.0f), CMath::Random(-0.01f, 0.1f));
	if (m_pPlayer == nullptr)
	{
		return E_POINTER;
	}

	// 3D
	m_p3DObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::NONE);
	if (m_p3DObject==nullptr)
	{
		return E_POINTER;
	}

	// X
	if (CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f), CObject::NONE) == nullptr)
	{
		return E_POINTER;
	}

	// Billboard
	if (CObjectBillboard::Create(D3DXVECTOR3(0.0f, 100.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), CObject::NONE) == nullptr)
	{
		return E_POINTER;
	}

	// タイム
	m_pTime = CTime::Create(D3DXVECTOR3(0.5f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2, 0.03f, 6);
	if (m_pTime == nullptr)
	{
		return E_POINTER;
	}

	// スコア
	m_pScore = CScore::Create(D3DXVECTOR3(0.9f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5, 0.03f, 6);
	if (m_pScore == nullptr)
	{
		return E_POINTER;
	}

	m_bPause = false; // ポーズ状態を初期化

	m_pTime->SetTime(120); // 時間セット
	m_pTime->CountDown(); // カウントダウン

	CSoundManager::GetInstance().Play(CSoundManager::LABEL_TEST_BGM); // BGM再生

	return S_OK;
}

//---------------------------------
// ゲームのエンド処理
//---------------------------------
void CManager::GameEnd(void)
{
	CSoundManager::GetInstance().Stop(); // サウンド停止

	// オブジェクト破棄
	CObject::ReleaseAll();
}

//---------------------------------
// ゲームのアンロード処理
//---------------------------------
void CManager::GameUnload(void)
{
	CEffect::Unload();    // Effect
	CExplosion::Unload(); // 爆発
	CBullet::Unload();    // バレット
	CEnemy::Unload();     // 敵
	CPlayer::Unload();    // プレイヤー
}

//-------------------
// レンダラーリセット
//-------------------
HRESULT CManager::RenererReset(const BOOL bWindow)
{
	if (m_pRenderer != nullptr)
	{
		HRESULT hr = m_pRenderer->ReSize(bWindow);// リサイズ処理

		return hr;
	}
	else
	{
		return E_POINTER;
	}
}