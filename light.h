//---------------------------------------
//
// ライト処理の定義 [light.h]
// Author: fuma sato
//
//---------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------
// ライトクラス
//-----------------
class CLight
{
// 公開
public:
	void Init(void);                //初期化処理
	void Uninit(void);              //終了処理
	void Update(void);              //更新処理
	size_t Set(D3DLIGHT9* pLight);  //ライト設定

// 非公開
private:
	array<D3DLIGHT9, 8> m_light; // ライト情報
};