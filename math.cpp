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
	angle = fmodf(angle, D3DX_PI * 2.0f); // -2πから2πの範囲に収める
	if (angle > D3DX_PI)
	{// 角度がπを超える場合
		angle -= D3DX_PI * 2.0f;
	}
	else if (angle < -D3DX_PI)
	{// 角度が-πを下回る場合
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//--------------------------------
// 角度の正規化
//--------------------------------
void CMath::NormalizeAngle(float* const pAngle)
{
	*pAngle = fmodf(*pAngle, D3DX_PI * 2.0f); // -2πから2πの範囲に収める
	if (*pAngle > D3DX_PI)
	{// 角度がπを超える場合
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle < -D3DX_PI)
	{// 角度が-πを下回る場合
		*pAngle += D3DX_PI * 2.0f;
	}
}

//--------------------------------
// 角度の正規化
//--------------------------------
D3DXVECTOR3 CMath::NormalizeRot(const D3DXVECTOR3 rot)
{
	D3DXVECTOR3 normalizedRot;                                             // 正規化された角度を格納する変数
	normalizedRot.x = NormalizeAngle(rot.x);                               // x軸の角度を正規化
	normalizedRot.y = NormalizeAngle(rot.y);                               // y軸の角度を正規化
	normalizedRot.z = NormalizeAngle(rot.z);                               // z軸の角度を正規化
	return D3DXVECTOR3(normalizedRot.x, normalizedRot.y, normalizedRot.z); // 正規化された角度を返す
}

//--------------------------------
// 角度の正規化
//--------------------------------
void CMath::NormalizeRot(D3DXVECTOR3* const pRot)
{
	NormalizeAngle(&pRot->x); // x軸の角度を正規化
	NormalizeAngle(&pRot->y); // y軸の角度を正規化
	NormalizeAngle(&pRot->z); // z軸の角度を正規化
}

//--------------------------------------
// 目的の角度にcoefficientで近づけていく (角度安全)
//--------------------------------------
void CMath::NormalizeAngleDeltaAndAdd(float* const pAngle, float destAngle, const float coefficient, const float deltaTime)
{
	NormalizeAngle(&destAngle);                                         // 目的の角度を正規化
	float diff = NormalizeAngle(destAngle - *pAngle);                   // 現在の角度と目的の角度の差を正規化
	*pAngle = NormalizeAngle(*pAngle + diff * coefficient * deltaTime); // 現在の角度に差を加算して正規化
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