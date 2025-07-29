//------------------------------------------
//
// 数値を扱う処理群 [math.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "math.h"

#include <type_traits>

//------------------------
// ローテーション
//------------------------
bool math::Rotation(Vector3 resultPos[], const D3DXVECTOR2 size, const float fAngle)
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
float math::NormalizeAngle(float angle)
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
void math::NormalizeAngle(float* const pAngle)
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
D3DXVECTOR3 math::NormalizeRot(const D3DXVECTOR3 rot)
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
void math::NormalizeRot(D3DXVECTOR3* const pRot)
{
	NormalizeAngle(&pRot->x); // x軸の角度を正規化
	NormalizeAngle(&pRot->y); // y軸の角度を正規化
	NormalizeAngle(&pRot->z); // z軸の角度を正規化
}

//--------------------------------------
// 目的の角度にcoefficientで近づけていく (角度安全)
//--------------------------------------
void math::NormalizeAngleDeltaAndAdd(float* const pAngle, float destAngle, const float coefficient, const float deltaTime)
{
	NormalizeAngle(&destAngle);                                         // 目的の角度を正規化
	float diff = NormalizeAngle(destAngle - *pAngle);                   // 現在の角度と目的の角度の差を正規化
	*pAngle = NormalizeAngle(*pAngle + diff * coefficient * deltaTime); // 現在の角度に差を加算して正規化
}

//---------------------------------
// 球座標
//---------------------------------
void math::SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length)
{
	pOutPos->x = basePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	pOutPos->y = basePos.y + cosf(rot.x) * Length;
	pOutPos->z = basePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//-------------------------------------------
// 整数の乱数を生成する
//-------------------------------------------
int math::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

//-------------------------------------------
// 実数の乱数を生成する
//-------------------------------------------
float math::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

//------------------------
// 方向ベクトルをオイラー角 (ピッチ、ヨー、ロール) に変換する関数
//------------------------
D3DXVECTOR3 math::VectorToEuler(const D3DXVECTOR3& direction)
{
	// 計算の安定のため、まずベクトルを正規化します
	D3DXVECTOR3 dir_normalized;
	D3DXVec3Normalize(&dir_normalized, &direction);

	D3DXVECTOR3 euler_angles;

	// --- Pitch (X軸周りの回転) の計算 ---
	// ベクトルのY成分から上下の傾きを計算します。
	// 正規化されたベクトルでは、Y成分がそのままピッチ角のサインになります (sin(angle) = y)。
	// D3Dでは一般的に下を向くのを正のピッチとするため、-yから計算します。
	euler_angles.x = -asinf(dir_normalized.y);

	// --- Yaw (Y軸周りの回転) の計算 ---
	// XZ平面上での水平方向の回転を計算します。
	// atan2f(x, z) を使うことで、360度全方位の角度を正しく扱えます。
	// Z軸の正方向を0度として、そこからの回転角度を求めます。
	euler_angles.y = atan2f(dir_normalized.x, dir_normalized.z);

	// --- Roll (Z軸周りの回転) の計算 ---
	// 方向ベクトルだけからはRoll（進行方向周りの捻り）は一意に定まりません。
	// 最も自然な「頭が上」の状態にするため、Rollは0とします。
	euler_angles.z = 0.0f;

	return euler_angles;
}
