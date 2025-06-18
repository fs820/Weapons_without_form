//------------------------------------------
//
// 数値を扱う処理群 [math.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "math.h"

#include <type_traits>

std::random_device CMath::rd;
std::mt19937 CMath::gen(CMath::rd());

//------------------------
// ローテーション
//------------------------
HRESULT CMath:: Rotation(D3DXVECTOR3 resultPos[], const D3DXVECTOR2 size, const float fAngle)
{
	D3DXVECTOR3 LocalPos[4] = {};
	LocalPos[0] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	LocalPos[1] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	LocalPos[2] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	LocalPos[3] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);

	D3DXMATRIX Mtx;
	D3DXMatrixRotationZ(&Mtx, fAngle);
	for (size_t cntPos = 0; cntPos < 4; cntPos++)
	{
		D3DXVec3TransformCoord(&resultPos[cntPos], &LocalPos[cntPos], &Mtx);
	}

	return S_OK;
}

//--------------------------------
// 角度の正規化
//--------------------------------
float CMath::NormalizeAngle(float angle)
{
	while (angle > D3DX_PI)
	{
		angle -= D3DX_PI * 2.0f;
	}
	while (angle < -D3DX_PI)
	{
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//--------------------------------
// 角度の正規化
//--------------------------------
void CMath::NormalizeAngle(float* pAngle)
{
	while (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
	while (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
}

//---------------------------------
// 球座標
//---------------------------------
void CMath::SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length)
{
	pOutPos->x = basePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	pOutPos->y = basePos.y + cosf(rot.x) * Length;
	pOutPos->z = basePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//-------------------------------------------
// 整数の乱数を生成する
//-------------------------------------------
int CMath::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

//-------------------------------------------
// 実数の乱数を生成する
//-------------------------------------------
float CMath::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}