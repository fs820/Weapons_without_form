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
#include "debug.h"
#include "sound.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "gui.h"
#include "hierarchy.h"
#include "motion.h"
#include "scene.h"

// Manager内部実装
namespace
{
	// テクスチャ
	const path m_textureFile = "data/TXT/texture.txt";                  // ファイルパス
	constexpr array<string_view, Index(TEXTAG::Max)> m_textureTagList = // タグリスト
	{
		"Number",
		"Ground",
		"Enemy1","Enemy2",
		"Bullet",
		"EX1","EX2",
		"Shadow_Effect"
	};

	// モデル
	const path m_modelFile = "data/TXT/model.txt";                      // ファイルパス
	constexpr array<string_view, Index(MODELTAG::Max)> m_modelTagList = // タグリスト
	{
		"XModel",
		"Shadow",
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
	const path m_motionFile = "data/TXT/motion.txt";                      // ファイルパス
	constexpr array<string_view, Index(MOTIONTAG::Max)> m_motionTagList = // タグリスト
	{
		"Player"
	};

	// 階層構造
	const path m_hierarchyFile = "data/TXT/hierarchy.txt";                      // ファイルパス
	constexpr array<string_view, Index(HIERARCHYTAG::Max)> m_hierarchyTagList = // タグリスト
	{
		"Player"
	};

	// プレイヤー
	const path m_playerFile = "data/TXT/player000.txt"; // ファイルパス

    // プレイヤー
    const path m_stageFile = "data/TXT/stage.txt"; // ファイルパス

	//----------------------------------
	// 
	// ロード関数
	// 
	//----------------------------------
	
    //---------------------------------
    // textureのロード処理
    //---------------------------------
	HRESULT LoadTexture(void)
	{
		// テクスチャのパスを取得する
		vector<path> textureList{};
		vector<D3DXVECTOR2> textureBlockList{};
		CFile* pFile = new CFile(m_textureFile); // テクスチャリストファイルを生成
		string readData = pFile->ReadText();     // テクスチャファイルの文字列を取得
		delete pFile;                            // ファイルを破棄
		pFile = nullptr;                         // ポインタをnullptrに

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
	HRESULT LoadModel(void)
	{
		// 規定モデルのパスを取得する
		vector<path> modelList{};
		CFile* pFile = new CFile(m_modelFile); // モデルリストファイルを生成
		string readData = pFile->ReadText();   // モデルファイルの文字列を取得
		delete pFile;                          // ファイルを破棄
		pFile = nullptr;                       // ポインタをnullptrに

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

		return S_OK; // 成功
	}

	//---------------------------------
	// モーションのロード処理
	//---------------------------------	
	HRESULT LoadMotion(void)
	{
		// 規定モデルのパスを取得する
		vector<path> modelList{};
		CFile* pFile = new CFile(m_motionFile); // モーションリストファイルを生成
		string readData = pFile->ReadText();    // モーションファイルの文字列を取得
		delete pFile;                           // ファイルを破棄
		pFile = nullptr;                        // ポインタをnullptrに

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
	HRESULT LoadHierarchy(void)
	{
		// 規定モデルのパスを取得する
		CFile* pFile = new CFile(m_hierarchyFile); // ヒエラルキーファイルを生成
		string readData = pFile->ReadText();       // 階層構造ファイルの文字列を取得
		delete pFile;                              // ファイルを破棄
		pFile = nullptr;                           // ポインタをnullptrに

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
    // modelのロード処理
    //---------------------------------
    HRESULT LoadStage(void)
    {
        // 規定モデルのパスを取得する
        vector<path> modelList{};
        CFile* pFile = new CFile(m_modelFile); // モデルリストファイルを生成
        string readData = pFile->ReadText();   // モデルファイルの文字列を取得
        delete pFile;                          // ファイルを破棄
        pFile = nullptr;                       // ポインタをnullptrに

        Index cnt{};
        while (true)
        {
        	auto block = ExtractBlock(readData, "Set", "End", cnt);
        	if (!block.has_value())
        	{// 取得できなかったら
        		break; // 終了
        	}
        	auto tag = StringToData<string>(block.value(), "Tag");
        	if (tag.has_value())
        	{
        		for (Index8 cntTag = 0; cntTag < Index8(MODELTAG::Max); cntTag++)
        		{
        			if (tag.value() == m_modelTagList[cntTag])
        			{
        				// 位置
        				optional<float> pos[3]{};
        				for (Index cnt = 0; cnt < 3; cnt++)
        				{
        					pos[cnt] = StringToData<float>(block.value(), "Pos", cnt);
        				}
        				if (pos[X].has_value() && pos[Y].has_value() && pos[Z].has_value())
        				{
        					D3DXVECTOR3 posData = { pos[X].value(),pos[Y].value(),pos[Z].value() };
        					CModelManager::GetInstance().SetPos(cntTag, posData);  // 位置をセット
        				}

        				// 回転
        				optional<float> rot[3]{};
        				for (Index cnt = 0; cnt < 3; cnt++)
        				{
        					rot[cnt] = StringToData<float>(block.value(), "Rot", cnt);
        				}
        				if (rot[X].has_value() && rot[Y].has_value() && rot[Z].has_value())
        				{
        					D3DXVECTOR3 rotData = { rot[X].value(),rot[Y].value(),rot[Z].value() };
        					CModelManager::GetInstance().SetRot(cntTag, rotData);     // 回転をセット
        				}

        				// スケール
        				optional<float> scale[3]{};
        				for (Index cnt = 0; cnt < 3; cnt++)
        				{
        					scale[cnt] = StringToData<float>(block.value(), "Scale", cnt);
        				}
        				if (scale[X].has_value() && scale[Y].has_value() && scale[Z].has_value())
        				{
        					D3DXVECTOR3 scaleData = { scale[X].value(),scale[Y].value(),scale[Z].value() };
        					CModelManager::GetInstance().SetScale(cntTag, scaleData); // スケールをセット
        				}
        			}
        		}
        	}
        	cnt++; // 次のブロックへ
        }

        return S_OK; // 成功
    }
}

//------------------------------
//
// マネージャークラス
//
//------------------------------

//---------------
// 初期化
//---------------
HRESULT CManager::Init(const context::InitContext initContext)
{
	// カメラ
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;

		if (m_pCamera != nullptr)
		{
			m_pCamera->Init();
		}
	}

	// ライト
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;

		if (m_pLight != nullptr)
		{
			m_pLight->Init();
		}
	}

	// Gui
	if (m_pGui == nullptr)
	{
		m_pGui = new CGui;

		if (m_pGui != nullptr)
		{
			m_pGui->Init();
		}
	}

	//// インプット
	//if (FAILED(CInput::GetInstance().Init(initContext.hWnd)))return E_FAIL;

	// サウンド
	if (FAILED(CSoundManager::GetInstance().Init(initContext.hWnd)))return E_FAIL;

	// デバック表示
	if (FAILED(CDebugProc::Init()))return E_FAIL;

	// 読み込み
	if (FAILED(LoadTexture()))return E_FAIL;   // テクスチャ
	if (FAILED(LoadModel()))return E_FAIL;     // モデル
	if (FAILED(LoadMotion()))return E_FAIL;    // モーション
	if (FAILED(LoadHierarchy()))return E_FAIL; // 階層構造
    if (FAILED(LoadStage()))return E_FAIL;     // ステージ

	SetMode(scene::MODE::Title);

	return S_OK;
}

//---------------
// 終了
//---------------
void CManager::Uninit(void)
{
	// モデル
	CModelManager::GetInstance().Release();

	// モーション
	CMotionManager::GetInstance().Release();

	// デバック表示
	CDebugProc::Uninit();

	// サウンド
	CSoundManager::GetInstance().Uninit();

	// モデル
	CModelManager::GetInstance().Unload();

	// テクスチャ
	CTextureManager::GetInstance().Unload();

	// インプット
	CInput::GetInstance().Uninit();

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
}

//---------------
// 更新
//---------------
void CManager::Update(const context::UpdateContext updateContext)
{
	//// インプット更新
	//CInput::GetInstance().Update();

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

	m_pGui->Update(); // Gui更新

	// サウンド更新
	CSoundManager::GetInstance().Update();
}

//---------------
// 描画
//---------------
HRESULT CManager::Draw(const context::DrawContext drawContext)
{

	return S_OK;
}

//-------------------
// モードのセット
//-------------------
HRESULT CManager::SetMode(scene::MODE mode)
{
	CSoundManager::GetInstance().Stop(); // サウンド停止
	CObject::ReleaseAll();               // オブジェクト破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Create(mode); // シーンの生成
	}
	else
	{
		return E_POINTER; // ポインタがnullptr
	}

	return S_OK; // 成功
}
