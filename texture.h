//------------------------------------------
//
// �e�N�X�`�������̒�` [texture.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------------------------------
// �e�N�X�`���N���X
//-----------------------------------------
class CTexture final
{
// ���J
public:
	CTexture(const path Path, const D3DXVECTOR2 block) :m_path(Path), m_pTexture{}, m_size{}, m_block(block), m_blockSize{}, m_countUse{} {}
	~CTexture() = default;

	HRESULT Init(void);
	void Uninit(void);
	path GetPath(void) const { return m_path; }
	D3DXVECTOR2 GetBlock(void) const { return m_block; }
	LPDIRECT3DTEXTURE9 GetTexture(void) const { return m_pTexture; }
	D3DXVECTOR2 GetSize(void) const { return m_size; }
	D3DXVECTOR2 GetBlockSize(void) const { return m_blockSize; }
	void AddUseCount(void) { m_countUse++; }

// ����J
private:
	const path m_path;             // �p�X
	const D3DXVECTOR2 m_block;     // �u���b�N��

	LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���|�C���^
	D3DXVECTOR2 m_size;            // �T�C�Y
	D3DXVECTOR2 m_blockSize;       // 1�u���b�N���̃T�C�Y
	size_t m_countUse;             // �ǂꂾ���g���Ă邩
};

//----------------------------------------------
// �e�N�X�`���Ǘ��N���X �V���O���g�� �R�s�[�֎~
//----------------------------------------------
class CTextureManager final
{
// ���J
public:
	static CTextureManager& GetInstance(void) { return m_instance; }

	CTextureManager(const CTextureManager&) = delete;
	CTextureManager& operator=(const CTextureManager&) = delete;

	HRESULT Load(vector<path> texturePassList, vector<D3DXVECTOR2> textureBlockList);
	void Unload(void);
	Index Register(const path sTexturePath, const D3DXVECTOR2 block = D3DXVECTOR2(1.0f, 1.0f));
	path GetPath(Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetPath() : path(); }
	D3DXVECTOR2 GetBlock(Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetBlock() : D3DXVECTOR2(~0, ~0); }
	LPDIRECT3DTEXTURE9 GetTexture(Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetTexture() : nullptr; }
	D3DXVECTOR2 GetSize(Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetSize() : D3DXVECTOR2(~0, ~0); }
	D3DXVECTOR2 GetBlockSize(Index idx) const { return (idx >= 0 && idx < m_apTexture.size()) ? m_apTexture[idx]->GetBlockSize() : D3DXVECTOR2(~0, ~0); }

// ����J
private:
	static CTextureManager m_instance;  // �����̃C���X�^���X
	
	CTextureManager() : m_apTexture{} {}
	~CTextureManager() = default;

	vector<CTexture*> m_apTexture; // �e�N�X�`�����
};