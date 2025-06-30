//---------------------------------------
//
//カメラの定義・宣言[camera.h]
//Author fuma sato
//
//---------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-------------------
// カメラクラス
//-------------------
class CCamera final
{
	// 公開
public:
	//カメラ構造体
	struct Camera
	{
		D3DXVECTOR3 posV;
		D3DXVECTOR3 posVDest;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 posRDest;
		D3DXVECTOR3 posU;
		D3DXVECTOR3 rot;
		D3DXMATRIX mtxProjection;
		D3DXMATRIX mtxView;
		float fDistance;
		float fDistanceMin;
		float fDistanceMax;
		D3DVIEWPORT9 viewport;
	};

	void Init(void);                //初期化処理
	void Uninit(void);              //終了処理
	void Update(void);              //更新処理
	void Set(Index idx);            //描画処理
	D3DXVECTOR3 GetPos(Index idx) const; //カメラの位置を取得
	const D3DVIEWPORT9* GetViewport(Index idx) const { return &m_camera[idx].viewport; }; //カメラの位置を取得
	void View(Index idx);
	void Game(Index idx);

	size_t GetCameraNum(void) const { return size_t(CAMERA_XNUM * CAMERA_YNUM); }

	// 非公開
private:
	static constexpr int CAMERA_XNUM = 1;                      //カメラの数X
	static constexpr int CAMERA_YNUM = 1;                      //カメラの数Y
	static constexpr float CAMERA_VIEW_SPEED = 0.01f;          //回転スピード
	static constexpr float CAMERA_ROT_SPEED = 0.05f;           //回転スピード
	static constexpr float CAMERA_SPEED = 2.0f;                //移動スピード
	static constexpr float CAMERA_INA = 0.05f;                 //追従倍率
	static constexpr float CAMERA_ROTX = 0.2f;                 //縦回転範囲
	static constexpr float CAMERA_ROTX_GRA = 0.01f;            //縦回転地面スペース
	static constexpr float CAMERA_DISTANCE_SPEED = 5.0f;       //ズームスピード
	static constexpr float CAMERA_DISTANCE_MIN = 5.0f;         //ズーム制限
	static constexpr float CAMERA_DISTANCE_MAX = 100.0f;       //ズーム制限
	static constexpr float CAMERA_DISTANCE_VIEW_MAX = 1000.0f; //ズーム制限
	static constexpr float CAMERA_WIDTH = 90.0f;               //視野の広さ
	static constexpr float CAMERA_MIN = 1.0f;                  //視野の最小
	static constexpr float CAMERA_MAX = 1000.0f;               //視野の最大

	Camera m_camera[CAMERA_XNUM * CAMERA_YNUM];
};