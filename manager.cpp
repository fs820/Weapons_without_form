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
#include "hierarchy.h"
#include "motion.h"

//------------------------------
//
// マネージャークラス
//
//------------------------------

// 静的メンバ変数の定義

// テクスチャ
const path CManager::m_textureFile = "data/TXT/texture.txt";       // ファイルパス
const string_view CManager::m_textureTagList[Index(TEXTAG::Max)] = // タグリスト
{
	"Number",
	"Ground",
	"Enemy1","Enemy2",
	"Bullet",
	"EX1","EX2",
	"Shadow_Effect"
};

// モデル
const path CManager::m_modelFile = "data/TXT/model.txt";           // ファイルパス
const string_view CManager::m_modelTagList[Index(MODELTAG::Max)] = // タグリスト
{
	"XModel",
	"PBody",
	"PHead",
	"PRArm",
	"PRHand",
	"PLArm",
	"PLHand",
	"PRLeg",
	"PRFoot",
	"PLLeg",
	"PLFoot",
	"PHat"
};

// モーション
const path CManager::m_motionFile = "data/TXT/motion.txt";           // ファイルパス
const string_view CManager::m_motionTagList[Index(MOTIONTAG::Max)] = // タグリスト
{
	"Player"
};

// 階層構造
const path CManager::m_hierarchyFile = "data/TXT/hierarchy.txt";           // ファイルパス
const string_view CManager::m_hierarchyTagList[Index(HIERARCHYTAG::Max)] = // タグリスト
{
	"Player"
};

// プレイヤー
const path CManager::m_playerFile = "data/TXT/player000.txt"; // ファイルパス

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

	//// サウンド
	//if (FAILED(CSoundManager::GetInstance().Init(CMain::GethWnd())))return E_FAIL;

	// デバック表示
	if (FAILED(CDebugProc::Init()))return E_FAIL;

	// 読み込み
	if (FAILED(LoadTexture()))return E_FAIL;   // テクスチャ
	if (FAILED(LoadModel()))return E_FAIL;     // モデル
	if (FAILED(LoadMotion()))return E_FAIL;    // モーション
	if (FAILED(LoadHierarchy()))return E_FAIL; // 階層構造

	// ゲーム
	if (FAILED(LoadGame()))return E_FAIL;  // ロード
	if (FAILED(StartGame()))return E_FAIL; // スタート

	return S_OK;
}

//---------------
// 終了
//---------------
void CManager::Uninit(void)
{
	EndGame();    // ゲームの終了
	UnloadGame(); // ソースの破棄

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
		EndGame();   // 終わらせる
		StartGame(); // 改めてスタート処理
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
HRESULT CManager::Draw(void)
{
	// レンダラーの描画
	if (m_pRenderer != nullptr)
	{
		if (FAILED(m_pRenderer->Draw())) { return E_FAIL; }
	}

	return S_OK;
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

//---------------------------------
// textureのロード処理
//---------------------------------
HRESULT CManager::LoadTexture()
{
	// テクスチャのパスを取得する
	vector<path> textureList{};
	vector<D3DXVECTOR2> textureBlockList{};
	m_pFile->ChangeFile(m_textureFile);    // テクスチャリストファイルをセット
	string readData = m_pFile->ReadText(); // テクスチャファイルの文字列を取得
	for (const auto& tag : m_textureTagList)
	{// 読み込見たいフラグ
		// フラグを基にパスとブロック数を取得する
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			textureList.push_back(sPath.value());
		}

		auto blockX = StringToData<float>(readData, tag, 1);
		auto blockY = StringToData<float>(readData, tag, 2);
		if (blockX.has_value() && blockY.has_value())
		{
			D3DXVECTOR2 textureBlock{ blockX.value(),blockY.value() };
			textureBlockList.push_back(textureBlock);
		}
	}
	return CTextureManager::GetInstance().Load(textureList, textureBlockList); // テクスチャを読み込む
}

//---------------------------------
// modelのロード処理
//---------------------------------
HRESULT CManager::LoadModel()
{
    // 規定モデルのパスを取得する
	vector<path> modelList{};
	m_pFile->ChangeFile(m_modelFile);    // テクスチャリストファイルをセット
	string readData = m_pFile->ReadText(); // テクスチャファイルの文字列を取得
	for (const auto& tag : m_modelTagList)
	{// 読み込見たいフラグ
		path modelPath{};
		// フラグを基にパスとブロック数を取得する
		auto sPath = StringToData<string>(readData, tag);
		if (sPath.has_value())
		{
			modelList.push_back(sPath.value());
		}
	}
	if (FAILED(CModelManager::GetInstance().Load(modelList)))return E_FAIL;   // 規定モデル

	//Index cnt{};
	//while (true)
	//{
	//	auto block = ExtractBlock(readData, "SetModel", "SetModelEnd", cnt);
	//	if (!block.has_value())
	//	{// 取得できなかったら
	//		break; // 終了
	//	}
	//	auto tag = StringToData<string>(block.value(), "Tag");
	//	if (tag.has_value())
	//	{
	//		for (Index cntTag = 0; cntTag < Index(MODELTAG::MAX); cntTag++)
	//		{
	//			if (tag.value() == m_modelTagList[cntTag])
	//			{
	//				// 位置
	//				optional<float> pos[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					pos[cnt] = StringToData<float>(block.value(), "Pos", cnt);
	//				}
	//				if (pos[X].has_value() && pos[Y].has_value() && pos[Z].has_value())
	//				{
	//					D3DXVECTOR3 posData = { pos[X].value(),pos[Y].value(),pos[Z].value() };
	//					CModelManager::GetInstance().SetPos(cntTag, posData);  // 位置をセット
	//				}

	//				// 回転
	//				optional<float> rot[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					rot[cnt] = StringToData<float>(block.value(), "Rot", cnt);
	//				}
	//				if (rot[X].has_value() && rot[Y].has_value() && rot[Z].has_value())
	//				{
	//					D3DXVECTOR3 rotData = { rot[X].value(),rot[Y].value(),rot[Z].value() };
	//					CModelManager::GetInstance().SetRot(cntTag, rotData);     // 回転をセット
	//				}

	//				// スケール
	//				optional<float> scale[3]{};
	//				for (Index cnt = 0; cnt < 3; cnt++)
	//				{
	//					scale[cnt] = StringToData<float>(block.value(), "Scale", cnt);
	//				}
	//				if (scale[X].has_value() && scale[Y].has_value() && scale[Z].has_value())
	//				{
	//					D3DXVECTOR3 scaleData = { scale[X].value(),scale[Y].value(),scale[Z].value() };
	//					CModelManager::GetInstance().SetScale(cntTag, scaleData); // スケールをセット
	//				}
	//			}
	//		}
	//	}
	//	cnt++; // 次のブロックへ
	//}

	return S_OK; // 成功
}

//---------------------------------
// モーションのロード処理
//---------------------------------	
HRESULT CManager::LoadMotion(void)
{
	// 規定モデルのパスを取得する
	vector<path> modelList{};
	m_pFile->ChangeFile(m_motionFile);    // モーションリストファイルをセット
	string readData = m_pFile->ReadText(); // モーションファイルの文字列を取得

	vector<motion::Source> aSource{};
	for (const auto& tag : m_motionTagList)
	{
		auto block = ExtractBlock(readData, tag, string_view(string(tag) + "End"));
		if (!block.has_value())
		{// 取得できなかったら
			break; // 終了
		}

		motion::Source source{};
		Index cntMotion{};
		while (true)
		{
			auto motionBlock = ExtractBlock(block.value(), "Motion", "MotionEnd", cntMotion);
			if (!motionBlock.has_value())
			{
				break;
			}

			motion::Motion Motion{};

			auto loop = StringToData<bool>(motionBlock.value(), "Loop");
			if (!loop.has_value())
			{
				break;
			}
			Motion.bLoop = loop.value();

			Index cntKey{};
			while (true)
			{
				auto keyBlock = ExtractBlock(motionBlock.value(), "Key", "KeyEnd", cntKey);
				if (!keyBlock.has_value())
				{
					break;
				}

				motion::Key key{};

				auto frame = StringToData<Index>(keyBlock.value(), "Frame");
				if (!frame.has_value())
				{
					break;
				}
				key.frame = frame.value();

				Index cntParts{};
				while (true)
				{
					auto partsBlock = ExtractBlock(keyBlock.value(), "Set", "End", cntParts);
					if (!partsBlock.has_value())
					{
						break;
					}

					motion::Parts parts{};

					// 位置
					optional<float> pos[3]{};
					for (Index cnt = 0; cnt < 3; cnt++)
					{
						pos[cnt] = StringToData<float>(partsBlock.value(), "Pos", cnt);
					}
					if (pos[X].has_value() && pos[Y].has_value() && pos[Z].has_value())
					{
						parts.pos = { pos[X].value(),pos[Y].value(),pos[Z].value() };
					}

					// 回転
					optional<float> rot[3]{};
					for (Index cnt = 0; cnt < 3; cnt++)
					{
						rot[cnt] = StringToData<float>(partsBlock.value(), "Rot", cnt);
					}
					if (rot[X].has_value() && rot[Y].has_value() && rot[Z].has_value())
					{
						parts.rot = { rot[X].value(),rot[Y].value(),rot[Z].value() };
					}
					key.aParts.push_back(parts); // パーツ情報追加
					++cntParts; // 次のパーツへ
				}
				Motion.aKey.push_back(key); // キー情報追加
				++cntKey; // 次のキーへ
			}
			source.aMotion.push_back(Motion); // モーション情報追加
			++cntMotion; // 次のモーションへ
		}
		aSource.push_back(source); // ソース追加
	}

	CMotionManager::GetInstance().Load(aSource); // モーションを読み込む

	return S_OK; // 成功
}

//---------------------------------
// ヒエラルキーのロード処理
//---------------------------------
HRESULT CManager::LoadHierarchy(void)
{
	// 規定モデルのパスを取得する
	m_pFile->ChangeFile(m_hierarchyFile);  // 階層構造ファイルをセット
	string readData = m_pFile->ReadText(); // 階層構造ファイルの文字列を取得

	for (const auto& tag : m_hierarchyTagList)
	{// 読み込見たいフラグ
		auto Hierarchy = ExtractBlock(readData, tag, string_view(string(tag) + "End"));

		vector < hierarchy::PartsInfo> hierarchy{}; // 階層構造情報

		Index cnt{};
		while (true)
		{
			hierarchy::PartsInfo partsInfo{}; // パーツ情報

			auto block = ExtractBlock(Hierarchy.value(), "Set", "End", cnt);
			if (!block.has_value())
			{// 取得できなかったら
				break; // 終了
			}
			auto tag = StringToData<string>(block.value(), "Tag");
			if (tag.has_value())
			{
				for (Index cntTag = 0; cntTag < Index(MODELTAG::Max); cntTag++)
				{
					if (tag.value() == m_modelTagList[cntTag])
					{
						partsInfo.modelTag = cntTag; // モデルのインデックスを追加
					}
				}
			}
			auto Parent = StringToData<int>(block.value(), "Parent");
			if (Parent.has_value())
			{
				partsInfo.parentIdx = Parent.value(); // モデルのインデックスを追加
			}
			float offSet[3]{};
			for (size_t cntOffSet = 0; cntOffSet < 3; cntOffSet++)
			{
				auto OffSet = StringToData<float>(block.value(), "OffSet", cntOffSet);
				if (OffSet.has_value())
				{
					offSet[cntOffSet] = OffSet.value();
				}
				else
				{
					offSet[cntOffSet] = 0.0f; // デフォルト値
				}
			}
			partsInfo.offSet = { offSet[X], offSet[Y], offSet[Z] }; // モデルのインデックスを追加

			hierarchy.push_back(partsInfo); // パーツ情報を追加
			cnt++; // 次のブロックへ
		}

		CHierarchyManager::GetInstance().Register(hierarchy); // 階層構造情報を登録
	}

	return S_OK; // 成功
}

//---------------------------------
// ゲームのロード処理
//---------------------------------
HRESULT CManager::LoadGame(void)
{
	return S_OK;
}

//---------------------------------
// ゲームのアンロード処理
//---------------------------------
void CManager::UnloadGame(void)
{
	CModelManager::GetInstance().Release();
	CMotionManager::GetInstance().Release();
}

//---------------------------------
// ゲームのスタート処理
//---------------------------------
HRESULT CManager::StartGame(void)
{
	//--------------------
	// インスタンス生成
	//--------------------

	// プレイヤー
	float scale = CMath::Random(1.0f, 1.0f);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, RandomAngle, 0.0f), D3DXVECTOR3(scale, scale, scale), CMath::Random(100.0f, 500.0f), CMath::Random(-0.01f, 0.1f));
	if (m_pPlayer == nullptr)
	{
		return E_POINTER;
	}

	// 3D
	m_p3DObject = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::TYPE::None, 2);
	if (m_p3DObject==nullptr)
	{
		return E_POINTER;
	}

	// X
	if (CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f), CObject::TYPE::None) == nullptr)
	{
		return E_POINTER;
	}

	// Billboard
	if (CObjectBillboard::Create(D3DXVECTOR3(0.0f, 10.0f, 10.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), CObject::TYPE::None, 2) == nullptr)
	{
		return E_POINTER;
	}

	// タイム
	m_pTime = CTime::Create(D3DXVECTOR3(0.5f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 2, 0.03f, 7);
	if (m_pTime == nullptr)
	{
		return E_POINTER;
	}

	// スコア
	m_pScore = CScore::Create(D3DXVECTOR3(0.9f, 0.05f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 5, 0.03f, 7);
	if (m_pScore == nullptr)
	{
		return E_POINTER;
	}

	m_bPause = false; // ポーズ状態を初期化

	m_pTime->SetTime(120); // 時間セット
	m_pTime->CountDown(); // カウントダウン

	CSoundManager::GetInstance().Play(Index8(sound::LABEL::TestBgm)); // BGM再生

	return S_OK;
}

//---------------------------------
// ゲームのエンド処理
//---------------------------------
void CManager::EndGame(void)
{
	CSoundManager::GetInstance().Stop(); // サウンド停止

	// オブジェクト破棄
	CObject::ReleaseAll();
}