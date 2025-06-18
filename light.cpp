//---------------------------------------
//
//���C�g����[light.cpp]
//Author fuma sato
//
//---------------------------------------
#include "light.h"
#include "manager.h"
#include "math.h"

//*****************************************
//���C�g�̏�����
//*****************************************
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	// ���C�g���N���A����
	ZeroMemory(m_light, sizeof(m_light));

	for (size_t cntLight = 0; cntLight < MAX_LIGHT; cntLight++)
	{
		// ���C�g�̎�ނ�ݒ�
		m_light[cntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_light[cntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		// ���C�g�̕���
		D3DXVECTOR3 vecDir = D3DXVECTOR3(CMath::Random(-1.0f, 1.0f), CMath::Random(-1.0f, 1.0f), CMath::Random(-1.0f, 1.0f));
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_light[cntLight].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(cntLight, &m_light[cntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(cntLight, TRUE);
	}

	////���
	//g_light[3].Type = D3DLIGHT_POINT;
	////�g�U��
	//g_light[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////����
	//g_light[3].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////���ʔ��ˌ�
	//g_light[3].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
	////�ʒu
	//g_light[3].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////�͈�
	//g_light[3].Range = 0.0f;
	////����������(���)
	//g_light[3].Attenuation0 = 0.1f;
	////����������(����)
	//g_light[3].Attenuation1 = 0.0f;
	////����������(����*2)
	//g_light[3].Attenuation2 = 0.0f;
	////���C�g�Z�b�g
	//pDevice->SetLight(3, &g_light[3]);
	////�L����
	//pDevice->LightEnable(3, FALSE);

	////���
	//g_light[nCnt].Type = D3DLIGHT_SPOT;
	////�g�U��
	//g_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////����
	//g_light[nCnt].Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////���ʔ��ˌ�
	//g_light[nCnt].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	////�ʒu
	//g_light[nCnt].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	////����
	//vecDir[nCnt] = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	////���K��
	//D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
	//g_light[nCnt].Direction = vecDir[nCnt];
	////�͈�
	//g_light[nCnt].Range = 0.0f;
	////�R�[��������
	//g_light[nCnt].Falloff = 0.01f;
	////����������
	//g_light[nCnt].Attenuation0 = 0.1f;
	////�R�[���p�x(��)
	//g_light[nCnt].Theta = D3DXToRadian(90.0f);
	////�R�[���p�x(�O)
	//g_light[nCnt].Phi = D3DXToRadian(360.0f);
	////���C�g�Z�b�g
	//pDevice->SetLight(nCnt, &g_light[nCnt]);
	////�L����
	//pDevice->LightEnable(nCnt, FALSE);
}

//*****************************************
//���C�g�̏I��
//*****************************************
void CLight::Uninit(void)
{
	
}

//*****************************************
//���C�g�̍X�V
//*****************************************
void CLight::Update(void)
{

}

//--------------------------
//�Z�b�g
//--------------------------
size_t CLight::Set(D3DLIGHT9* pLight)
{
	return 0;
}