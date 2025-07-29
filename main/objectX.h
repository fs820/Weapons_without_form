//------------------------------------------
//
// オブジェクトXの処理の定義・宣言[objectX.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "object.h"
#include "manager.h"

//-----------------------------------------
// オブジェクトXクラス (オブジェクト派生)
//-----------------------------------------
class CObjectX : public CObject
{
// 公開
public:
	CObjectX() : m_modelIdx{} {};
	CObjectX(Index priority) : CObject(priority), m_modelIdx{} {}
	virtual ~CObjectX() = default;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTAG model, TYPE type);
	HRESULT Init(DirectX::Transform transform, MODELTAG model, TYPE type);
	void Uninit(void) override;
	void Draw(const context::DrawContext drawContext) override;

// 非公開
private:
	void OnCollision(const CObject& other) override {};

	Index m_modelIdx;  // モデルインデックス
};