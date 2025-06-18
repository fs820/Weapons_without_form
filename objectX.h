//------------------------------------------
//
// オブジェクトXの処理の定義・宣言[objectX.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"

using namespace DirectX3D; // DirectX3D名前空間の使用

//-----------------------------------------
// オブジェクトXクラス (オブジェクト派生)
//-----------------------------------------
class CObjectX : public CObject
{
// 公開
public:
	CObjectX() : m_mtxWorld{} {}
	CObjectX(int priority) : CObject(priority), m_mtxWorld{} {}
	~CObjectX() = default;

	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type, int priority = 3);
	static CObjectX* Create(Transform transform, TYPE type, int priority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, TYPE type);
	HRESULT Init(Transform transform, TYPE type);
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

// 非公開
private:
	void OnCollision(const CObject& other) override {};

	static size_t m_modelIdx;           // モデル番号

	D3DXMATRIX m_mtxWorld;              // マトリックス
};