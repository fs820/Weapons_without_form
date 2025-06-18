//---------------------------------------
//
//ライト処理[light.cpp]
//Author fuma sato
//
//---------------------------------------
#include "light.h"
#include "manager.h"
#include "math.h"

//*****************************************
//ライトの初期化
//*****************************************
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	// ライトをクリアする
	ZeroMemory(m_light, sizeof(m_light));

	for (size_t cntLight = 0; cntLight < MAX_LIGHT; cntLight++)
	{
		// ライトの種類を設定
		m_light[cntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_light[cntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		// ライトの方向
		D3DXVECTOR3 vecDir = D3DXVECTOR3(CMath::Random(-1.0f, 1.0f), CMath::Random(-1.0f, 1.0f), CMath::Random(-1.0f, 1.0f));
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_light[cntLight].Direction = vecDir;

		// ライトを設定する
		pDevice->SetLight(cntLight, &m_light[cntLight]);

		// ライトを有効にする
		pDevice->LightEnable(cntLight, TRUE);
	}

	////種類
	//g_light[3].Type = D3DLIGHT_POINT;
	////拡散光
	//g_light[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////環境光
	//g_light[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////鏡面反射光
	//g_light[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////位置
	//g_light[3].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////範囲
	//g_light[3].Range = 0.0f;
	////距離減衰率(一定)
	//g_light[3].Attenuation0 = 0.1f;
	////距離減衰率(距離)
	//g_light[3].Attenuation1 = 0.0f;
	////距離減衰率(距離*2)
	//g_light[3].Attenuation2 = 0.0f;
	////ライトセット
	//pDevice->SetLight(3, &g_light[3]);
	////有効化
	//pDevice->LightEnable(3, FALSE);

	////種類
	//g_light[nCnt].Type = D3DLIGHT_SPOT;
	////拡散光
	//g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////環境光
	//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////鏡面反射光
	//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////位置
	//g_light[nCnt].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////方向
	//vecDir[nCnt] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	////正規化
	//D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
	//g_light[nCnt].Direction = vecDir[nCnt];
	////範囲
	//g_light[nCnt].Range = 0.0f;
	////コーン減衰率
	//g_light[nCnt].Falloff = 0.01f;
	////距離減衰率
	//g_light[nCnt].Attenuation0 = 0.1f;
	////コーン角度(内)
	//g_light[nCnt].Theta = D3DXToRadian(90.0f);
	////コーン角度(外)
	//g_light[nCnt].Phi = D3DXToRadian(360.0f);
	////ライトセット
	//pDevice->SetLight(nCnt, &g_light[nCnt]);
	////有効化
	//pDevice->LightEnable(nCnt, FALSE);
}

//*****************************************
//ライトの終了
//*****************************************
void CLight::Uninit(void)
{
	
}

//*****************************************
//ライトの更新
//*****************************************
void CLight::Update(void)
{

}

//--------------------------
//セット
//--------------------------
size_t CLight::Set(D3DLIGHT9* pLight)
{
	return 0;
}