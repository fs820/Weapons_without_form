//------------------------------------------
//
// シーン [scene.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "scene.h"
#include "game.h"

using namespace scene; // シーン空間の使用

//-----------------------------
// 
// シーンクラス
// 
//-----------------------------

//------------------------------
// 生成処理
//------------------------------
CScene* CScene::Create(MODE mode)
{
	CScene* pScene{};
	switch (mode)
	{
	case MODE::Title:
		break;
	case MODE::Game:
		pScene = new CGame; // ゲームシーンの生成
		break;
	case MODE::Result:
		break;
	}
	if (pScene == nullptr)
	{
		return nullptr; // 生成失敗
	}

	if (FAILED(pScene->Init()))
	{
		delete pScene;    // 初期化失敗なら削除
		pScene = nullptr; // ポインタをnullptrにする
		return nullptr;   // 生成失敗
	}
	return pScene; // 成功
}