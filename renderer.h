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
// �`��֌W
//----------------------------
namespace renderer
{
	// �����_�[�^�[�Q�b�g
	enum class RENDER_TARGET : Index8
	{
		BackBuffer,
		Main,
		BackGround,
		Char,
		Effect,
		UI,
		Afterimage,
		Max
	};
}

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

	// ��
	inline constexpr Color White(float alpha = 1.0f) { return D3DCOLOR_ARGB(255, 255, 255, Index8(float(255) * alpha)); }
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
	struct Vertex
	{
		Vector3 pos; // ���W
		float rhw;   // ���W�ϊ��W��(1.0f)
		Color col;   // �J���[
		Vector2 tex; // �e�N�X�`�����W

		Vertex() : pos{ 0.0f,0.0f,0.0f }, rhw{ 1.0f }, col{ WHITE }, tex{ 0.0f,0.0f } {}
		~Vertex() = default;
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
	 struct Vertex
	 {
		 Vector3 pos; // ���W
		 Vector3 nor; // �@���x�N�g��
		 Color col;   // �J���[
		 Vector2 tex; // �e�N�X�`�����W

		 Vertex() : pos{ 0.0f,0.0f,0.0f }, nor{ 0.0f,0.0f,0.0f }, col{ WHITE }, tex{ 0.0f,0.0f } {}
		 ~Vertex() = default;
	 };
 }

//------------------
// �����_���[�N���X
//------------------
class CRenderer
{
// ���J
public:
	CRenderer() : m_pD3D{}, m_pD3DDevice{}, m_d3dpp{},
		m_pVtx{}, m_pBackBuffer{}, m_pBackBufferZBuffer{}, m_backBufferViewport{},
		m_pTexture{}, m_pRender{}, m_pZBuff{}, m_viewport{},
		m_pBackGroundTexture{}, m_pBackGroundRender{}, m_pBackGroundZBuff{}, m_backGroundViewport{},
		m_pCharTexture{}, m_pCharRender{}, m_pCharZBuff{}, m_charViewport{},
		m_pEffectTexture{}, m_pEffectRender{}, m_pEffectZBuff{}, m_effectViewport{},
		m_pUITexture{}, m_pUIRender{}, m_pUIZBuff{}, m_UIViewport{},
		m_pAfterimageTexture{}, m_pAfterimageRender{}, m_pAfterimageZBuff{}, m_afterimageViewport{} {}
	~CRenderer() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow = TRUE);
	void Uninit(void);
	void Update(void) const;
	HRESULT Draw(void);
	
	HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, span<const DirectX2D::Vertex> vertex, DirectX::Vector3 offSet = {0.0f,0.0f,0.0f}, float angle = 0.0f);
	HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, DirectX::Vector2 size, DirectX::Vector3 pos = { 0.0f,0.0f,0.0f }, float angle = 0.0f, DirectX::Color color = DirectX::WHITE, DirectX::Vector2 texBlock = { 1.0f,1.0f });

	const LPDIRECT3DDEVICE9& GetDevice(void) const { return m_pD3DDevice; };
	void SetRender(void) const;
	void SetFog(const bool bFog, D3DFOGMODE mode = D3DFOG_LINEAR, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255), const float start = 100.0f, const float end = 300.0f) const;
	HRESULT GetBackBufferSize(D3DXVECTOR2* size) const;
	HRESULT GetBackBufferToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT GetTargetSize(D3DXVECTOR2* size) const;
	HRESULT GetTargetToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT GetViewportSize(D3DXVECTOR2* size) const;
	HRESULT GetViewportToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT ReSize(const BOOL bWindow);

	D3DXVECTOR2 GetCursorPos(void) const;
	HRESULT GetCursorPos(D3DXVECTOR2* pOutpos) const;

	HRESULT ChangeTarget(renderer::RENDER_TARGET target);
	LPDIRECT3DTEXTURE9 GetTexture(void) const;
	// ����J
private:
	void ResetRelease(void);
	HRESULT ResetCreate(void);

	HRESULT RenderTexture(LPDIRECT3DTEXTURE9 pTexture, D3DCOLOR color = DirectX::WHITE, bool blendOne = false, bool zEnable = true, bool bAlphaTest = false, bool bPoint = false);

	HRESULT SetTexture(void);

	LPDIRECT3D9 m_pD3D;              // DirectX�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;  // DirectX�f�o�C�X

	D3DPRESENT_PARAMETERS m_d3dpp;     // DirectX�p�����[�^

	// �e�N�X�`���`��p
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;            // �`��p�|���S��

	// �o�b�N�o�b�t�@
	LPDIRECT3DSURFACE9 m_pBackBuffer;        // �o�b�N�o�b�t�@
	LPDIRECT3DSURFACE9 m_pBackBufferZBuffer; // �o�b�N�o�b�t�@��Z�o�b�t�@
	D3DVIEWPORT9       m_backBufferViewport; // �`��͈͂��w�肷��r���[�|�[�g

	// ���C���e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pTexture; // �`���ƂȂ�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pRender;  // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pZBuff;   // Z�o�b�t�@
	D3DVIEWPORT9       m_viewport; // �`��͈͂��w�肷��r���[�|�[�g

	// �w�i�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pBackGroundTexture; // �`���ƂȂ�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pBackGroundRender;  // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pBackGroundZBuff;   // Z�o�b�t�@
	D3DVIEWPORT9       m_backGroundViewport; // �`��͈͂��w�肷��r���[�|�[�g

	// �L�����N�^�[�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pCharTexture; // �`���ƂȂ�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pCharRender;  // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pCharZBuff;   // Z�o�b�t�@
	D3DVIEWPORT9       m_charViewport; // �`��͈͂��w�肷��r���[�|�[�g

	// �G�t�F�N�g�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pEffectTexture; // �`���ƂȂ�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pEffectRender;  // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pEffectZBuff;   // Z�o�b�t�@
	D3DVIEWPORT9       m_effectViewport; // �`��͈͂��w�肷��r���[�|�[�g

	// UI�e�N�X�`��
	LPDIRECT3DTEXTURE9 m_pUITexture; // �`���ƂȂ�e�N�X�`��
	LPDIRECT3DSURFACE9 m_pUIRender;  // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pUIZBuff;   // Z�o�b�t�@
	D3DVIEWPORT9       m_UIViewport; // �`��͈͂��w�肷��r���[�|�[�g

	// �c���p�e�N�X�`��
	array<LPDIRECT3DTEXTURE9, 2> m_pAfterimageTexture; // �`���ƂȂ�e�N�X�`��
	array <LPDIRECT3DSURFACE9, 2> m_pAfterimageRender; // �`����s�����߂̕\�ʁi�T�[�t�F�X�j
	LPDIRECT3DSURFACE9 m_pAfterimageZBuff;             // Z�o�b�t�@
	D3DVIEWPORT9       m_afterimageViewport;           // �`��͈͂��w�肷��r���[�|�[�g
};