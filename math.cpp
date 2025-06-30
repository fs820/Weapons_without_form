//------------------------------------------
//
// ”’l‚ğˆµ‚¤ˆ—ŒQ [math.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "math.h"

#include <type_traits>

std::random_device CMath::rd;
std::mt19937 CMath::gen(CMath::rd());

//------------------------
// ƒ[ƒe[ƒVƒ‡ƒ“
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
// Šp“x‚Ì³‹K‰»
//--------------------------------
float CMath::NormalizeAngle(float angle)
{
	angle = fmodf(angle, D3DX_PI * 2.0f); // -2ƒÎ‚©‚ç2ƒÎ‚Ì”ÍˆÍ‚Éû‚ß‚é
	if (angle > D3DX_PI)
	{// Šp“x‚ªƒÎ‚ğ’´‚¦‚éê‡
		angle -= D3DX_PI * 2.0f;
	}
	else if (angle < -D3DX_PI)
	{// Šp“x‚ª-ƒÎ‚ğ‰º‰ñ‚éê‡
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//--------------------------------
// Šp“x‚Ì³‹K‰»
//--------------------------------
void CMath::NormalizeAngle(float* const pAngle)
{
	*pAngle = fmodf(*pAngle, D3DX_PI * 2.0f); // -2ƒÎ‚©‚ç2ƒÎ‚Ì”ÍˆÍ‚Éû‚ß‚é
	if (*pAngle > D3DX_PI)
	{// Šp“x‚ªƒÎ‚ğ’´‚¦‚éê‡
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle < -D3DX_PI)
	{// Šp“x‚ª-ƒÎ‚ğ‰º‰ñ‚éê‡
		*pAngle += D3DX_PI * 2.0f;
	}
}

//--------------------------------
// Šp“x‚Ì³‹K‰»
//--------------------------------
D3DXVECTOR3 CMath::NormalizeRot(const D3DXVECTOR3 rot)
{
	D3DXVECTOR3 normalizedRot;                                             // ³‹K‰»‚³‚ê‚½Šp“x‚ğŠi”[‚·‚é•Ï”
	normalizedRot.x = NormalizeAngle(rot.x);                               // x²‚ÌŠp“x‚ğ³‹K‰»
	normalizedRot.y = NormalizeAngle(rot.y);                               // y²‚ÌŠp“x‚ğ³‹K‰»
	normalizedRot.z = NormalizeAngle(rot.z);                               // z²‚ÌŠp“x‚ğ³‹K‰»
	return D3DXVECTOR3(normalizedRot.x, normalizedRot.y, normalizedRot.z); // ³‹K‰»‚³‚ê‚½Šp“x‚ğ•Ô‚·
}

//--------------------------------
// Šp“x‚Ì³‹K‰»
//--------------------------------
void CMath::NormalizeRot(D3DXVECTOR3* const pRot)
{
	NormalizeAngle(&pRot->x); // x²‚ÌŠp“x‚ğ³‹K‰»
	NormalizeAngle(&pRot->y); // y²‚ÌŠp“x‚ğ³‹K‰»
	NormalizeAngle(&pRot->z); // z²‚ÌŠp“x‚ğ³‹K‰»
}

//--------------------------------------
// –Ú“I‚ÌŠp“x‚Écoefficient‚Å‹ß‚Ã‚¯‚Ä‚¢‚­ (Šp“xˆÀ‘S)
//--------------------------------------
void CMath::NormalizeAngleDeltaAndAdd(float* const pAngle, float destAngle, const float coefficient, const float deltaTime)
{
	NormalizeAngle(&destAngle);                                         // –Ú“I‚ÌŠp“x‚ğ³‹K‰»
	float diff = NormalizeAngle(destAngle - *pAngle);                   // Œ»İ‚ÌŠp“x‚Æ–Ú“I‚ÌŠp“x‚Ì·‚ğ³‹K‰»
	*pAngle = NormalizeAngle(*pAngle + diff * coefficient * deltaTime); // Œ»İ‚ÌŠp“x‚É·‚ğ‰ÁZ‚µ‚Ä³‹K‰»
}

//---------------------------------
// ‹…À•W
//---------------------------------
void CMath::SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length)
{
	pOutPos->x = basePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	pOutPos->y = basePos.y + cosf(rot.x) * Length;
	pOutPos->z = basePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//-------------------------------------------
// ®”‚Ì—”‚ğ¶¬‚·‚é
//-------------------------------------------
int CMath::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

//-------------------------------------------
// À”‚Ì—”‚ğ¶¬‚·‚é
//-------------------------------------------
float CMath::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}