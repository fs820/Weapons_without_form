//----------------------------------------------------
//
// テクスチャ処理 [texture.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "texture.h"
#include "manager.h"

//----------------------------------------------------
//
// テクスチャクラス
//
//----------------------------------------------------

//----------------------------------------------------
// 初期化処理
//----------------------------------------------------
HRESULT CTexture::Init(void)
{
	// テクスチャ
	D3DXIMAGE_INFO imageInfo = {};
	if (FAILED(D3DXGetImageInfoFromFile
	(
		m_path.c_str(),
		&imageInfo
	)))
	{
		return E_FAIL;
	}
	m_size = D3DXVECTOR2((float)imageInfo.Width, (float)imageInfo.Height); // サイズを取得
	m_blockSize = D3DXVECTOR2(m_size.x / m_block.x, m_size.y / m_block.y); // 1ブロック分のサイズ
	m_countUse = 0;                                                        // 何枚使ってるの

	if (FAILED(D3DXCreateTextureFromFile
	(
		pDevice,
		m_path.c_str(),
		&m_pTexture
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CTexture::Uninit(void)
{
	if (m_pTexture != nullptr)
	{// あれば
		m_pTexture->Release(); // 開放
		m_pTexture = nullptr;  // null
	}
}

//----------------------------------------------------
//
// テクスチャ管理クラス
//
//----------------------------------------------------

// 静的メンバ変数
CTextureManager CTextureManager::m_instance{};     // 自分のインスタンス

//----------------------------------------------------
// 初期化処理
//----------------------------------------------------
HRESULT CTextureManager::Load(std::vector<path> texturePassList, std::vector<D3DXVECTOR2> textureBlockList)
{
	// vectorのサイズを事前に確保
	if (m_apTexture.size() < texturePassList.size())
	{
		m_apTexture.resize(texturePassList.size());
	}

	// 規定テクスチャの生成
	for (size_t cntTex = 0; cntTex < m_apTexture.size(); cntTex++)
	{// 規定タイプ回す
		if (m_apTexture[cntTex] == nullptr)
		{// なければ
			m_apTexture[cntTex] = new CTexture(texturePassList[cntTex], textureBlockList[cntTex]); // 生成

			if (m_apTexture[cntTex] != nullptr)
			{// あれば
				if (FAILED(m_apTexture[cntTex]->Init()))
				{
					m_apTexture[cntTex]->Uninit();
					delete m_apTexture[cntTex];
					m_apTexture[cntTex] = nullptr;
					return E_FAIL;
				}
				m_apTexture[cntTex]->AddUseCount();                     // 使っている数の追加
			}
			else
			{
				return E_FAIL; // エラー値
			}
		}
	}

	return S_OK;
}

//----------------------------------------------------
// 終了処理
//----------------------------------------------------
void CTextureManager::Unload(void)
{
	for (auto& pTeture : m_apTexture)
	{// 全てのテクスチャ
		if (pTeture != nullptr)
		{// あれば
			pTeture->Uninit(); // 終了
			delete pTeture;    // 開放
			pTeture = nullptr; // null
		}
	}
}

//----------------------------------------------------
// 更新
//----------------------------------------------------
size_t CTextureManager::Register(const path sTexturePath, const D3DXVECTOR2 block)
{
	if (sTexturePath.empty())
	{
		return INVALID_ID; // エラー値
	}

	// 同一テクスチャ確認
	for (size_t cntTexture = 0; cntTexture < m_apTexture.size(); cntTexture++)
	{// 今までのテクスチャ
		if (m_apTexture[cntTexture]->GetPath() == sTexturePath)
		{// 同じテクスチャを発見
			m_apTexture[cntTexture]->AddUseCount(); // 使っている数の追加
			return cntTexture;                      // そのテクスチャつかってね
		}
	}

	// 新テクスチャの生成
	CTexture* pTexture = new CTexture(sTexturePath, block);
	if (FAILED(pTexture->Init()))
	{
		pTexture->Uninit();
		delete pTexture;
		pTexture = nullptr;
		return INVALID_ID;
	}
	else
	{
		pTexture->AddUseCount(); // 使っている数の追加
		m_apTexture.push_back(pTexture); // 追加
		return m_apTexture.size() - 1;
	}
}
