//------------------------------------------
//
// ���l�����������Q [math.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "math.h"

#include <type_traits>

std::random_device CMath::rd;
std::mt19937 CMath::gen(CMath::rd());

//------------------------
// ���[�e�[�V����
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
// �p�x�̐��K��
//--------------------------------
float CMath::NormalizeAngle(float angle)
{
	angle = fmodf(angle, D3DX_PI * 2.0f); // -2�΂���2�΂͈̔͂Ɏ��߂�
	if (angle > D3DX_PI)
	{// �p�x���΂𒴂���ꍇ
		angle -= D3DX_PI * 2.0f;
	}
	else if (angle < -D3DX_PI)
	{// �p�x��-�΂������ꍇ
		angle += D3DX_PI * 2.0f;
	}
	return angle;
}

//--------------------------------
// �p�x�̐��K��
//--------------------------------
void CMath::NormalizeAngle(float* const pAngle)
{
	*pAngle = fmodf(*pAngle, D3DX_PI * 2.0f); // -2�΂���2�΂͈̔͂Ɏ��߂�
	if (*pAngle > D3DX_PI)
	{// �p�x���΂𒴂���ꍇ
		*pAngle -= D3DX_PI * 2.0f;
	}
	else if (*pAngle < -D3DX_PI)
	{// �p�x��-�΂������ꍇ
		*pAngle += D3DX_PI * 2.0f;
	}
}

//--------------------------------
// �p�x�̐��K��
//--------------------------------
D3DXVECTOR3 CMath::NormalizeRot(const D3DXVECTOR3 rot)
{
	D3DXVECTOR3 normalizedRot;                                             // ���K�����ꂽ�p�x���i�[����ϐ�
	normalizedRot.x = NormalizeAngle(rot.x);                               // x���̊p�x�𐳋K��
	normalizedRot.y = NormalizeAngle(rot.y);                               // y���̊p�x�𐳋K��
	normalizedRot.z = NormalizeAngle(rot.z);                               // z���̊p�x�𐳋K��
	return D3DXVECTOR3(normalizedRot.x, normalizedRot.y, normalizedRot.z); // ���K�����ꂽ�p�x��Ԃ�
}

//--------------------------------
// �p�x�̐��K��
//--------------------------------
void CMath::NormalizeRot(D3DXVECTOR3* const pRot)
{
	NormalizeAngle(&pRot->x); // x���̊p�x�𐳋K��
	NormalizeAngle(&pRot->y); // y���̊p�x�𐳋K��
	NormalizeAngle(&pRot->z); // z���̊p�x�𐳋K��
}

//--------------------------------------
// �ړI�̊p�x��coefficient�ŋ߂Â��Ă��� (�p�x���S)
//--------------------------------------
void CMath::NormalizeAngleDeltaAndAdd(float* const pAngle, float destAngle, const float coefficient, const float deltaTime)
{
	NormalizeAngle(&destAngle);                                         // �ړI�̊p�x�𐳋K��
	float diff = NormalizeAngle(destAngle - *pAngle);                   // ���݂̊p�x�ƖړI�̊p�x�̍��𐳋K��
	*pAngle = NormalizeAngle(*pAngle + diff * coefficient * deltaTime); // ���݂̊p�x�ɍ������Z���Đ��K��
}

//---------------------------------
// �����W
//---------------------------------
void CMath::SpherePos(D3DXVECTOR3* const pOutPos, const D3DXVECTOR3 basePos, const D3DXVECTOR3 rot, const float Length)
{
	pOutPos->x = basePos.x + sinf(rot.x) * sinf(rot.y) * Length;
	pOutPos->y = basePos.y + cosf(rot.x) * Length;
	pOutPos->z = basePos.z + sinf(rot.x) * cosf(rot.y) * Length;
}

//-------------------------------------------
// �����̗����𐶐�����
//-------------------------------------------
int CMath::RandomInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

//-------------------------------------------
// �����̗����𐶐�����
//-------------------------------------------
float CMath::RandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}