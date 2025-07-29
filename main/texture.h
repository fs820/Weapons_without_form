//------------------------------------------
//
// テクスチャ処理の定義 [texture.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include <vector>
#include <filesystem>
#include "renderer.h"
#include "common.h"

//-----------------------------------------
// テクスチャクラス
//-----------------------------------------
class CTexture final
{
// 公開
public:
	CTexture(const std::filesystem::path Path, const D3DXVECTOR2 block) :m_path(Path), m_pTexture{}, m_size{}, m_block(block), m_blockSize{}, m_countUse{} {}
	~CTexture() = default;

	HRESULT Init(void);
	void Uninit(void);
    std::filesystem::path GetPath(void) const { return m_path; }
	D3DXVECTOR2 GetBlock(void) const { return m_block; }
	LPDIRECT3DTEXTURE9 GetTexture(void) const { return m_pTexture; }
	D3DXVECTOR2 GetSize(void) const { return m_size; }
	D3DXVECTOR2 GetBlockSize(void) const { return m_blockSize; }
	void AddUseCount(void) { m_countUse++; }

// 非公開
private:
	const std::filesystem::path m_path;             // パス
	const D3DXVECTOR2 m_block;     // ブロック数

	LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャポインタ
	D3DXVECTOR2 m_size;            // サイズ
	D3DXVECTOR2 m_blockSize;       // 1ブロック分のサイズ
	size_t m_countUse;             // どれだけ使ってるか
};

//----------------------------------------------
// テクスチャ管理クラス
//----------------------------------------------
class CTextureManager final
{
// 公開
public:
    CTextureManager() : m_apTexture{} {}
    ~CTextureManager() = default;

	CTextureManager(const CTextureManager&) = delete;
	CTextureManager& operator=(const CTextureManager&) = delete;
    CTextureManager(const CTextureManager&&) = default;
    CTextureManager& operator=(const CTextureManager&&) = default;

	HRESULT Load(std::vector<std::filesystem::path> texturePassList, std::vector<D3DXVECTOR2> textureBlockList);
	void Unload(void);
    common::Index Register(const std::filesystem::path sTexturePath, const D3DXVECTOR2 block = D3DXVECTOR2(1.0f, 1.0f));
    std::filesystem::path GetPath(common::Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetPath() : std::filesystem::path(); }
	D3DXVECTOR2 GetBlock(common::Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetBlock() : D3DXVECTOR2(~0, ~0); }
	LPDIRECT3DTEXTURE9 GetTexture(common::Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetTexture() : nullptr; }
	D3DXVECTOR2 GetSize(common::Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetSize() : D3DXVECTOR2(~0, ~0); }
	D3DXVECTOR2 GetBlockSize(common::Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetBlockSize() : D3DXVECTOR2(~0, ~0); }

// 非公開
private:
	std::vector<CTexture*> m_apTexture; // テクスチャ情報
};
