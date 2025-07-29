//------------------------------------------
//
// Numberの処理の定義・宣言[number.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//--------------------------
// 数字 (末端)
//--------------------------
class CNumber final
{
// 公開
public:
	CNumber() : m_pVtxBuff{}, m_number{}, m_size{} {}
	~CNumber() = default;

	static CNumber* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int startNumber);

	static void Digit(int* aNumber, const int nData, const size_t digits);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int startNumber);
	void Uninit(void);
	void Draw(void);

	void SetNumber(const int number);
	void AddNumber(const int number);
	int GetNumber(void) const { return m_number; }

	D3DXVECTOR2 GetSize(void) const { return m_size; }

// 非公開
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点
	int m_number;                       // 数値
	D3DXVECTOR2 m_size;                 // 1数字のサイズ
};