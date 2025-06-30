//------------------------------------------
//
//�`��p�̏����̒�`�E�錾[renderer.h]
//Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"

//----------------------------
// DirectX���C�u����
//----------------------------
#include "d3dx9.h"

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")


//----------------------------
// �e��}�N��
//----------------------------
#define BIT_DWORD(d) (bit_cast<DWORD>(d)) // �r�b�g��DWORD�^�ɕϊ�����}�N��

//----------------------------
// DirectX�萔
//----------------------------
namespace DirectX
{
	// ���O�̒u������
	using Vector2 = D3DXVECTOR2;
	using Vector3 = D3DXVECTOR3;
	using Vector4 = D3DXVECTOR4;
	using Matrix = D3DXMATRIX;
	using Color = D3DCOLOR;
	using ColorX = D3DXCOLOR;

	// �g�����X�t�H�[���\����
	struct Transform
	{
		Vector3 pos;     // �ʒu
		Vector3 rot;     // ��]
		Vector3 scale;   // �X�P�[��
		Matrix mtxWorld; // �}�g���b�N�X

		Transform() : pos{ 0.0f, 0.0f, 0.0f }, rot{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f }, mtxWorld{} {}
		Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : pos(position), rot(rotation), scale(scale), mtxWorld{} {}
		~Transform() = default;

		void Set(const Vector3& pos, const Vector3& rot, const Vector3& scale)
		{
			this->pos = pos;
			this->rot = rot;
			this->scale = scale;
			D3DXMatrixIdentity(&mtxWorld); // �}�g���b�N�X�̏�����
		}

		void Set(const Transform& transform)
		{
			pos = transform.pos;
			rot = transform.rot;
			scale = transform.scale;
			mtxWorld = transform.mtxWorld; // �}�g���b�N�X�̃R�s�[
		}

		void SetMatrix(void)
		{
			D3DXMATRIX mtxRot, mtxTrans, mtxScale;
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);        // ���s�ړ��s��̐���
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z); // ��]�s��̐���
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);      // �X�P�[�����O�s��̐���
			mtxWorld = mtxScale * mtxRot * mtxTrans;                      // �}�g���b�N�X�̍���
		}

		Transform& operator=(const Transform& rhs)
		{
			if (this != &rhs)
			{
				Set(rhs);
			}
			return *this;
		}
		Transform& operator+=(const Transform& rhs)
		{
			pos += rhs.pos;
			rot += rhs.rot;
			scale += rhs.scale;
			return *this;
		}
		Transform& operator-=(const Transform& rhs)
		{
			pos -= rhs.pos;
			rot -= rhs.rot;
			scale -= rhs.scale;
			return *this;
		}
	};

	// ���_
	constexpr int VT_DEF = 4;// ��{���_��

	// �F�̒萔
	constexpr Color WHITE = D3DCOLOR_ARGB(255, 255, 255, 255); // ��
	constexpr Color RED = D3DCOLOR_ARGB(255, 255, 0, 0);       // ��
	constexpr Color GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);     // ��
	constexpr Color BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);      // ��
	constexpr Color BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);       // ��

	// ������ translucent
	constexpr Color TRANSLUCENT_WHITE = D3DCOLOR_ARGB(128, 255, 255, 255); // ��
	constexpr Color TRANSLUCENT_RED = D3DCOLOR_ARGB(128, 255, 0, 0);       // ��
	constexpr Color TRANSLUCENT_GREEN = D3DCOLOR_ARGB(128, 0, 255, 0);     // ��
	constexpr Color TRANSLUCENT_BLUE = D3DCOLOR_ARGB(128, 0, 0, 255);      // ��
	constexpr Color TRANSLUCENT_BLACK = D3DCOLOR_ARGB(128, 0, 0, 0);       // ��

	// ���� transparency
	constexpr Color TRANSPARENCY_WHITE = D3DCOLOR_ARGB(0, 255, 255, 255); // ��
	constexpr Color TRANSPARENCY_RED = D3DCOLOR_ARGB(0, 255, 0, 0);       // ��
	constexpr Color TRANSPARENCY_GREEN = D3DCOLOR_ARGB(0, 0, 255, 0);     // ��
	constexpr Color TRANSPARENCY_BLUE = D3DCOLOR_ARGB(0, 0, 0, 255);      // ��
	constexpr Color TRANSPARENCY_BLACK = D3DCOLOR_ARGB(0, 0, 0, 0);       // ��
}

//----------------------------
// DirectX2D�萔
//----------------------------
 namespace DirectX2D
{
	using namespace DirectX; // DirectX�̖��O��Ԃ��g�p

	// ���_
	constexpr DWORD FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; // ���_�t�H�[�}�b�g

	// ���_���̍\���̂̒�`
	using Vertex = struct
	{
		Vector3 pos; // ���W
		float rhw;   // ���W�ϊ��W��(1.0f)
		Color col;   // �J���[
		Vector2 tex; // �e�N�X�`�����W
	};
}

 //----------------------------
// DirectX3D�萔
//----------------------------
 namespace DirectX3D
 {
	 using namespace DirectX; // DirectX�̖��O��Ԃ��g�p

	 // ���_
	 constexpr DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1; // ���_�t�H�[�}�b�g

	 // ���_���̍\���̂̒�`
	 using Vertex = struct
	 {
		 Vector3 pos; // ���W
		 Vector3 nor; // �@���x�N�g��
		 Color col;   // �J���[
		 Vector2 tex; // �e�N�X�`�����W
	 };
 }

//------------------
// �����_���[�N���X
//------------------
class CRenderer
{
// ���J
public:
	CRenderer() : m_pD3D{}, m_pD3DDevice{}, m_d3dpp{} {}
	~CRenderer() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow = TRUE);
	void Uninit(void);
	void Update(void) const;
	void Draw(void) const;

	const LPDIRECT3DDEVICE9& GetDevice(void) const { return m_pD3DDevice; };
	void SetRender(void) const;
	void SetFog(const bool bFog, D3DFOGMODE mode = D3DFOG_LINEAR, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255), const float start = 100.0f, const float end = 300.0f) const;
	HRESULT GetDxScreenSize(D3DXVECTOR2* size) const;
	HRESULT GetDxScreenToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT ReSize(const BOOL bWindow);

	D3DXVECTOR2 GetCursorPos(void) const;
	HRESULT GetCursorPos(D3DXVECTOR2* pOutpos) const;
	// ����J
private:
	void ResetRelease(void) const;
	void ResetCreate(void) const;

	LPDIRECT3D9 m_pD3D;              // DirectX�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;  // DirectX�f�o�C�X

	D3DPRESENT_PARAMETERS m_d3dpp;     // DirectX�p�����[�^
};