//----------------------------------------
//
//カメラ処理[camera.cpp]
//Author fuma sato
//
//----------------------------------------
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "player.h"

using namespace camera; // カメラ空間の使用

//-----------------------------
// 
// カメラクラス
// 
//-----------------------------

//----------------------
//初期化処理
//----------------------
void CCamera::Init(void)
{
	for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
	{
		//カメラ設定
		m_camera[nCount].posV = D3DXVECTOR3(0.0f, 200.0f, -300.0f);
		m_camera[nCount].posVDest = D3DXVECTOR3(0.0f, 200.0f, -300.0f);
		m_camera[nCount].posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		m_camera[nCount].posRDest = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		m_camera[nCount].posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_camera[nCount].rot = D3DXVECTOR3(-atan2f(150.0f, 300.0f), 0.0f, 0.0f);
		m_camera[nCount].fDistance = sqrtf((m_camera[nCount].posR.x - m_camera[nCount].posV.x) * (m_camera[nCount].posR.x - m_camera[nCount].posV.x) + (m_camera[nCount].posR.y - m_camera[nCount].posV.y) * (m_camera[nCount].posR.y - m_camera[nCount].posV.y) + (m_camera[nCount].posR.z - m_camera[nCount].posV.z) * (m_camera[nCount].posR.z - m_camera[nCount].posV.z));
		m_camera[nCount].fDistanceMin = CAMERA_DISTANCE_MIN;
		m_camera[nCount].fDistanceMax = CAMERA_DISTANCE_MAX;
	}
}

//-------------------
//終了処理
//-------------------
void CCamera::Uninit(void)
{

}

//-------------------
//更新処理
//-------------------
void CCamera::Update(void)
{
	//for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
	//{
	//	if (!bPause())
	//	{
	//		float fMouseWheel;
	//		fMouseWheel = GetMouseWheel();
	//		m_camera[nCount].fDistance += fMouseWheel * CAMERA_DISTANCE_SPEED;
	//		if (GetKeyboardPress(DIK_1) || GetJoykeyPress(JOYKEY_RB, CONTROLLER_1))
	//		{
	//			m_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
	//		}
	//		if (GetKeyboardPress(DIK_2) || GetJoykeyPress(JOYKEY_RT, CONTROLLER_1))
	//		{
	//			m_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
	//		}

	//		if (!strcmp(ControllerName(CONTROLLER_1), ELE_CON))
	//		{
	//			if (GetdJoykeyPress(ELEKEY_RB, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
	//			}
	//			if (GetdJoykeyPress(ELEKEY_RT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
	//			}
	//		}
	//		else if (!strcmp(ControllerName(CONTROLLER_1), PS_CON))
	//		{
	//			if (GetdJoykeyPress(PSKEY_RB, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
	//			}
	//			if (GetdJoykeyPress(PSKEY_RT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
	//			}
	//		}
	//		else if (!strcmp(ControllerName(CONTROLLER_1), NIN_CON))
	//		{
	//			if (GetdJoykeyPress(NINKEY_RB, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
	//			}
	//			if (GetdJoykeyPress(NINKEY_RT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
	//			}
	//		}
	//		else if (!IsXInputControllerConnected(CONTROLLER_1) && IsDirectInputControllerConnected(CONTROLLER_1))
	//		{
	//			if (GetdJoykeyPress(DKEY_RB, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += CAMERA_DISTANCE_SPEED;
	//			}
	//			if (GetdJoykeyPress(DKEY_RT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].fDistance += -CAMERA_DISTANCE_SPEED;
	//			}
	//		}

	//		//スケール制限
	//		m_camera[nCount].fDistance = max(m_camera[nCount].fDistanceMin, min(m_camera[nCount].fDistanceMax, m_camera[nCount].fDistance));
	//	}

	//	if (GetMode() == MODE_TITLE || GetMode() == MODE_RESULT || GetMode() == MODE_RANK)
	//	{
	//		m_camera[nCount].rot.y += CAMERA_VIEW_SPEED;

	//		//スケール制限
	//		m_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX_GRA, m_camera[nCount].rot.x));

	//		//正規化
	//		if (m_camera[nCount].rot.x > D3DX_PI)
	//		{
	//			m_camera[nCount].rot.x -= D3DX_PI * 2.0f;
	//		}
	//		else if (m_camera[nCount].rot.x < -D3DX_PI)
	//		{
	//			m_camera[nCount].rot.x += D3DX_PI * 2.0f;
	//		}

	//		//正規化
	//		if (m_camera[nCount].rot.y > D3DX_PI)
	//		{
	//			m_camera[nCount].rot.y -= D3DX_PI * 2.0f;
	//		}
	//		else if (m_camera[nCount].rot.y < -D3DX_PI)
	//		{
	//			m_camera[nCount].rot.y += D3DX_PI * 2.0f;
	//		}

	//		if (GetKeyboardPress(DIK_R))
	//		{
	//			m_camera[nCount].posR.x = m_camera[nCount].posV.x + sinf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.y = m_camera[nCount].posV.y + sinf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.z = m_camera[nCount].posV.z + cosf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//		}
	//		else
	//		{
	//			m_camera[nCount].posV.x = m_camera[nCount].posR.x + sinf(m_camera[nCount].rot.y - D3DX_PI) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posV.y = m_camera[nCount].posR.y + sinf(m_camera[nCount].rot.x - D3DX_PI) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posV.z = m_camera[nCount].posR.z + cosf(m_camera[nCount].rot.y - D3DX_PI) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//		}
	//	}
	//	else if (!bPause())
	//	{
	//		Player* pPlayer = GetPlayer();
	//		float* pStick;
	//		float* pMouseMove;
	//		float Oldrot = m_camera[nCount].rot.y;
	//		//X
	//		if (IsXInputControllerConnected(CONTROLLER_1))
	//		{
	//			//右スティック処理
	//			pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
	//			if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
	//			{
	//				m_camera[nCount].rot.y += cosf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
	//				m_camera[nCount].rot.x += sinf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
	//			}
	//		}
	//		//d
	//		else if (IsDirectInputControllerConnected(CONTROLLER_1))
	//		{
	//			//右スティック処理
	//			pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
	//			if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
	//			{
	//				m_camera[nCount].rot.y += cosf(atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
	//				m_camera[nCount].rot.x += sinf(-atan2f(*(pStick + 1), *pStick)) * CAMERA_ROT_SPEED;
	//			}
	//		}

	//		pMouseMove = GetMouseMove();
	//		if (sqrtf(*pMouseMove * *pMouseMove + *(pMouseMove + 1) * *(pMouseMove + 1)) >= STICK_DED)
	//		{
	//			m_camera[nCount].rot.y += cosf(atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_ROT_SPEED;
	//			m_camera[nCount].rot.x += sinf(-atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_ROT_SPEED;
	//		}

	//		if (GetKeyboardPress(DIK_DOWN))
	//		{
	//			m_camera[nCount].rot.x -= CAMERA_ROT_SPEED;
	//		}

	//		if (GetKeyboardPress(DIK_UP))
	//		{
	//			m_camera[nCount].rot.x += CAMERA_ROT_SPEED;
	//		}

	//		if (GetKeyboardPress(DIK_LEFT))
	//		{
	//			m_camera[nCount].rot.y -= CAMERA_ROT_SPEED;
	//		}

	//		if (GetKeyboardPress(DIK_RIGHT))
	//		{
	//			m_camera[nCount].rot.y += CAMERA_ROT_SPEED;
	//		}

	//		if (pPlayer->move.x == 0.0f && pPlayer->move.z == 0.0f && Oldrot == m_camera[nCount].rot.y)
	//		{
	//			static int nCnt = 0;
	//			float rot = m_camera[nCount].rot.y;
	//			nCnt++;
	//			if (nCnt % 300 < 50)
	//			{
	//				rot = pPlayer->Destrot.y - D3DX_PI;
	//			}
	//			//正規化
	//			if (rot - m_camera[nCount].rot.y > D3DX_PI)
	//			{
	//				rot = m_camera[nCount].rot.y + ((rot - m_camera[nCount].rot.y) - D3DX_PI * 2.0f);
	//			}
	//			else if (rot - m_camera[nCount].rot.y < -D3DX_PI)
	//			{
	//				rot = m_camera[nCount].rot.y + ((rot - m_camera[nCount].rot.y) + D3DX_PI * 2.0f);
	//			}

	//			m_camera[nCount].rot.y += (rot - m_camera[nCount].rot.y) * CAMERA_INA;
	//		}

	//		if (!GetKeyboardPress(DIK_R) && !GetMousePress(MOUSE_B1))
	//		{
	//			//スケール制限
	//			m_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX_GRA, m_camera[nCount].rot.x));
	//		}

	//		//正規化
	//		if (m_camera[nCount].rot.x > D3DX_PI)
	//		{
	//			m_camera[nCount].rot.x -= D3DX_PI * 2.0f;
	//		}
	//		else if (m_camera[nCount].rot.x < -D3DX_PI)
	//		{
	//			m_camera[nCount].rot.x += D3DX_PI * 2.0f;
	//		}

	//		//正規化
	//		if (m_camera[nCount].rot.y > D3DX_PI)
	//		{
	//			m_camera[nCount].rot.y -= D3DX_PI * 2.0f;
	//		}
	//		else if (m_camera[nCount].rot.y < -D3DX_PI)
	//		{
	//			m_camera[nCount].rot.y += D3DX_PI * 2.0f;
	//		}

	//		if (GetKeyboardPress(DIK_R) || GetMousePress(MOUSE_B1))
	//		{
	//			//スケール制限
	//			m_camera[nCount].rot.x = max(-D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX * 0.5f, min(D3DX_PI * CAMERA_ROTX - D3DX_PI * CAMERA_ROTX * 0.5f, m_camera[nCount].rot.x));
	//			m_camera[nCount].posV = pPlayer->pos;
	//			m_camera[nCount].posV.y += pPlayer->aModel[0].pos.y + pPlayer->aModel[1].pos.y;
	//			m_camera[nCount].posR.x = m_camera[nCount].posV.x + sinf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.y = m_camera[nCount].posV.y + sinf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.z = m_camera[nCount].posV.z + cosf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//		}
	//		else
	//		{
	//			Player* pPlayer = GetPlayer();
	//			m_camera[nCount].posRDest.x = pPlayer->pos.x + sinf(atan2f(pPlayer->move.z, pPlayer->move.x)) * SPEED;
	//			m_camera[nCount].posRDest.y = pPlayer->pos.y + cosf(m_camera[nCount].rot.x) * SPEED;
	//			m_camera[nCount].posRDest.z = pPlayer->pos.z + cosf(atan2f(pPlayer->move.z, pPlayer->move.x)) * SPEED;
	//			m_camera[nCount].posVDest.x = pPlayer->pos.x + sinf(m_camera[nCount].rot.y - D3DX_PI) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posVDest.y = pPlayer->pos.y + sinf(m_camera[nCount].rot.x - D3DX_PI) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posVDest.z = pPlayer->pos.z + cosf(m_camera[nCount].rot.y - D3DX_PI) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;

	//			m_camera[nCount].posR.x += (m_camera[nCount].posRDest.x - m_camera[nCount].posR.x) * CAMERA_INA;
	//			m_camera[nCount].posR.y += (m_camera[nCount].posRDest.y - m_camera[nCount].posR.y) * CAMERA_INA;
	//			m_camera[nCount].posR.z += (m_camera[nCount].posRDest.z - m_camera[nCount].posR.z) * CAMERA_INA;
	//			m_camera[nCount].posV.x += (m_camera[nCount].posVDest.x - m_camera[nCount].posV.x) * CAMERA_INA;
	//			m_camera[nCount].posV.y += (m_camera[nCount].posVDest.y - m_camera[nCount].posV.y) * CAMERA_INA;
	//			m_camera[nCount].posV.z += (m_camera[nCount].posVDest.z - m_camera[nCount].posV.z) * CAMERA_INA;
	//		}

	//		if (GetKeyboardPress(DIK_P) || GetMousePress(MOUSE_B2) == true || GetJoykeyPress(JOYKEY_X, CONTROLLER_1) == true)
	//		{
	//			float* pStick;
	//			float* pMouseMove;
	//			//X
	//			if (IsXInputControllerConnected(CONTROLLER_1))
	//			{
	//				//右スティック処理
	//				pStick = GetJoyStick(STICK_RIGHT, CONTROLLER_1);
	//				if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
	//				{
	//					m_camera[nCount].posV.x += sinf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pStick + 1), -*pStick)) * CAMERA_SPEED;
	//					m_camera[nCount].posV.z += cosf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pStick + 1), -*pStick)) * CAMERA_SPEED;
	//				}
	//			}
	//			//d
	//			else if (IsDirectInputControllerConnected(CONTROLLER_1))
	//			{
	//				//右スティック処理
	//				pStick = GetdJoyStick(STICK_RIGHT, CONTROLLER_1);
	//				if (sqrtf(*pStick * *pStick + *(pStick + 1) * *(pStick + 1)) >= STICK_DED)
	//				{
	//					m_camera[nCount].posV.x += sinf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(-*(pStick + 1), -*pStick)) * CAMERA_SPEED;
	//					m_camera[nCount].posV.z += cosf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(-*(pStick + 1), -*pStick)) * CAMERA_SPEED;
	//				}
	//			}

	//			pMouseMove = GetMouseMove();
	//			if (sqrtf(*pMouseMove * *pMouseMove + *(pMouseMove + 1) * *(pMouseMove + 1)) >= STICK_DED)
	//			{
	//				m_camera[nCount].posV.x -= sinf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_SPEED;
	//				m_camera[nCount].posV.z -= cosf(m_camera[nCount].rot.y - D3DX_PI * 0.5f + atan2f(*(pMouseMove + 1), *pMouseMove)) * CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_LEFT) || GetJoykeyPress(JOYKEY_LEFT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.x += sinf(m_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
	//				m_camera[nCount].posV.z += cosf(m_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_RIGHT) || GetJoykeyPress(JOYKEY_RIGHT, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.x -= sinf(m_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
	//				m_camera[nCount].posV.z -= cosf(m_camera[nCount].rot.y - D3DX_PI * 0.5f) * CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_DOWN) || GetJoykeyPress(JOYKEY_DOWN, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.x += sinf(m_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
	//				m_camera[nCount].posV.z += cosf(m_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_UP) || GetJoykeyPress(JOYKEY_UP, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.x -= sinf(m_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
	//				m_camera[nCount].posV.z -= cosf(m_camera[nCount].rot.y - D3DX_PI) * CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_RSHIFT) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.y += CAMERA_SPEED;
	//			}

	//			if (GetKeyboardPress(DIK_RCONTROL) || GetJoykeyPress(JOYKEY_X, CONTROLLER_1))
	//			{
	//				m_camera[nCount].posV.y -= CAMERA_SPEED;
	//			}

	//			m_camera[nCount].posR.x = m_camera[nCount].posV.x + sinf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.y = m_camera[nCount].posV.y + sinf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//			m_camera[nCount].posR.z = m_camera[nCount].posV.z + cosf(m_camera[nCount].rot.y) * cosf(m_camera[nCount].rot.x) * m_camera[nCount].fDistance;
	//		}
	//	}
	//}
}

//-------------------
//描画処理
//-------------------
void CCamera::Set(Index idx, const context::DrawContext drawContext)
{
    LPDIRECT3DDEVICE9 pDevice = drawContext.pAppContext->pRenderer->GetDevice();

	//ビューマトリックス初期化
	D3DXMatrixIdentity(&m_camera[idx].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH
	(
		&m_camera[idx].mtxView,
		&m_camera[idx].posV,
		&m_camera[idx].posR,
		&m_camera[idx].posU
	);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera[idx].mtxView);

	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&m_camera[idx].mtxProjection);

	// スクリーンサイズ
	D3DXVECTOR2 screenSize{};
    drawContext.pAppContext->pRenderer->GetViewportSize(&screenSize);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH
	(
		&m_camera[idx].mtxProjection,
		D3DXToRadian(CAMERA_WIDTH),//視界の広さ
		screenSize.x / screenSize.y,//比率
		CAMERA_MIN,//どこから
		CAMERA_MAX//どこまで
	);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera[idx].mtxProjection);
}

//-------------------
// カメラの位置を取得
//-------------------
D3DXVECTOR3 CCamera::GetPos(Index idx)  const
{
	//カメラの位置を取得
	return m_camera[idx].posV;
}

//--------------------
//背景用
//--------------------
void CCamera::View(Index idx)
{
	if (idx == CAMERA_XNUM * CAMERA_YNUM)
	{
		for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
		{
			m_camera[nCount].posV = D3DXVECTOR3(0.0f, 120.0f, -6000.0f);
			m_camera[nCount].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_camera[nCount].fDistance = sqrtf((m_camera[nCount].posR.x - m_camera[nCount].posV.x) * (m_camera[nCount].posR.x - m_camera[nCount].posV.x) + (m_camera[nCount].posR.y - m_camera[nCount].posV.y) * (m_camera[nCount].posR.y - m_camera[nCount].posV.y) + (m_camera[nCount].posR.z - m_camera[nCount].posV.z) * (m_camera[nCount].posR.z - m_camera[nCount].posV.z));
			m_camera[nCount].fDistanceMax = CAMERA_DISTANCE_VIEW_MAX;
		}
		return;
	}

	m_camera[idx].posV = D3DXVECTOR3(0.0f, 120.0f, -6000.0f);
	m_camera[idx].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera[idx].fDistance = sqrtf((m_camera[idx].posR.x - m_camera[idx].posV.x) * (m_camera[idx].posR.x - m_camera[idx].posV.x) + (m_camera[idx].posR.y - m_camera[idx].posV.y) * (m_camera[idx].posR.y - m_camera[idx].posV.y) + (m_camera[idx].posR.z - m_camera[idx].posV.z) * (m_camera[idx].posR.z - m_camera[idx].posV.z));
	m_camera[idx].fDistanceMax = CAMERA_DISTANCE_VIEW_MAX;
}

//--------------------
//ゲーム用
//--------------------
void CCamera::Game(Index idx)
{
	if (idx == CAMERA_XNUM * CAMERA_YNUM)
	{
		for (int nCount = 0; nCount < CAMERA_XNUM * CAMERA_YNUM; nCount++)
		{
			m_camera[nCount].posV = D3DXVECTOR3(0.0f, 120.0f, -300.0f);
			m_camera[nCount].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_camera[nCount].fDistance = sqrtf((m_camera[nCount].posR.x - m_camera[nCount].posV.x) * (m_camera[nCount].posR.x - m_camera[nCount].posV.x) + (m_camera[nCount].posR.y - m_camera[nCount].posV.y) * (m_camera[nCount].posR.y - m_camera[nCount].posV.y) + (m_camera[nCount].posR.z - m_camera[nCount].posV.z) * (m_camera[nCount].posR.z - m_camera[nCount].posV.z));
			m_camera[nCount].fDistanceMax = CAMERA_DISTANCE_MAX;
		}
		return;
	}

	m_camera[idx].posV = D3DXVECTOR3(0.0f, 120.0f, -300.0f);
	m_camera[idx].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camera[idx].fDistance = sqrtf((m_camera[idx].posR.x - m_camera[idx].posV.x) * (m_camera[idx].posR.x - m_camera[idx].posV.x) + (m_camera[idx].posR.y - m_camera[idx].posV.y) * (m_camera[idx].posR.y - m_camera[idx].posV.y) + (m_camera[idx].posR.z - m_camera[idx].posV.z) * (m_camera[idx].posR.z - m_camera[idx].posV.z));
	m_camera[idx].fDistanceMax = CAMERA_DISTANCE_MAX;
}
