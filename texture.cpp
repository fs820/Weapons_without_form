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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// テクスチャ
	D3DXIMAGE_INFO imageInfo = {};
	if (FAILED(D3DXGetImageInfoFromFile
	(
		m_path.string().c_str(),
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
		m_path.string().c_str(),
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
HRESULT CTextureManager::Load(vector<path> texturePassList, vector<D3DXVECTOR2> textureBlockList)
{
	m_typePass = texturePassList;
	m_typeBlock = textureBlockList;

	// 規定テクスチャの生成
	for (size_t cntTex = 0; cntTex < m_typePass.size(); cntTex++)
	{// 規定タイプ回す
		if (m_apTexture[cntTex] == nullptr)
		{// なければ
			m_apTexture[cntTex] = new CTexture(m_typePass[cntTex], m_typeBlock[cntTex]); // 生成

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
				m_nNumAll++; // 足す
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
	for (size_t cntTexture = 0; cntTexture < m_nNumAll; cntTexture++)
	{// 全てのテクスチャ
		if (m_apTexture[cntTexture] != nullptr)
		{// あれば
			m_apTexture[cntTexture]->Uninit(); // 終了
			delete m_apTexture[cntTexture];    // 開放
			m_apTexture[cntTexture] = nullptr; // null
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
	for (size_t cntTexture = 0; cntTexture < m_nNumAll; cntTexture++)
	{// 今までのテクスチャ
		if (m_apTexture[cntTexture]->GetPath() == sTexturePath)
		{// 同じテクスチャを発見
			m_apTexture[cntTexture]->AddUseCount(); // 使っている数の追加
			return cntTexture;                      // そのテクスチャつかってね
		}
	}

	// 新テクスチャの生成
	if (m_apTexture[m_nNumAll] == nullptr)
	{// なければ
		m_apTexture[m_nNumAll] = new CTexture(sTexturePath, block); // 生成

		if (m_apTexture[m_nNumAll] != nullptr)
		{// あれば
			if (FAILED(m_apTexture[m_nNumAll]->Init()))
			{
				m_apTexture[m_nNumAll]->Uninit();
				delete m_apTexture[m_nNumAll];
				m_apTexture[m_nNumAll] = nullptr;
				return INVALID_ID;
			}
			m_apTexture[m_nNumAll]->AddUseCount();                         // 使っている数の追加
			return m_nNumAll++; // 足す前の値を返しその後足す
		}
		else
		{
			return INVALID_ID; // エラー値
		}
	}
	return INVALID_ID; // エラー値
}