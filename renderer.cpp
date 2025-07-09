//------------------------------------------
//
//�`��p�̏���[renderer.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "debug.h"
#include "camera.h"
#include "light.h"
#include "gui.h"

using namespace renderer; // �`��X�y�[�X�̎g�p
using namespace DirectX;  // DirectX�X�y�[�X�̎g�p

//---------------------------------------
//
// �`���{�N���X
//
//---------------------------------------

//------------------------------
//����������
//------------------------------
HRESULT CRenderer::Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow)
{
	//�I�u�W�F�N�g����
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION); // Direct3D�I�u�W�F�N�g�̐���
	if (m_pD3D==nullptr)
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h
	D3DDISPLAYMODE d3ddm = {};//�_�C���N�gX�f�B�X�v���C���[�h�̕ϐ��錾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �N���C�A���g�̈�̃T�C�Y���擾
	RECT clientRect = {};
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // �N���C�A���g�T�C�Y�̎擾

	//�o�b�N�o�b�t�@�̐ݒ�
	m_d3dpp.hDeviceWindow = hWnd;
	m_d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
	m_d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_d3dpp.Windowed = bWindow;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	m_d3dpp.FullScreen_RefreshRateInHz = m_d3dpp.Windowed ? 0 : d3ddm.RefreshRate;

	DWORD qualityLevels = 0;
	const D3DMULTISAMPLE_TYPE samples[] =
	{
		D3DMULTISAMPLE_16_SAMPLES,
		D3DMULTISAMPLE_8_SAMPLES,
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_2_SAMPLES,
		D3DMULTISAMPLE_NONE
	};

	for (auto sample : samples)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType
		(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			m_d3dpp.BackBufferFormat, m_d3dpp.Windowed,
			sample, &qualityLevels))
		)
		{
			m_d3dpp.MultiSampleType = sample;
			m_d3dpp.MultiSampleQuality = (sample == D3DMULTISAMPLE_NONE) ? 0 : qualityLevels - 1;
			break;
		}
	}

	//�f�o�C�X����
	if (FAILED(m_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice
	)))
	{
		if (FAILED(m_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice
		)))
		{
			if (FAILED(m_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	SetRender(); // �`��ݒ�

	// �e�N�X�`���`��
	if (FAILED(SetTexture())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CRenderer::Uninit(void)
{
	// ���_�o�b�t�@
	SAFE_RELEASE(m_pVtx);

	// Z�o�b�t�@
	SAFE_RELEASE(m_pAfterimageZBuff);
	SAFE_RELEASE(m_pBackGroundZBuff);
	SAFE_RELEASE(m_pCharZBuff);
	SAFE_RELEASE(m_pEffectZBuff);
	SAFE_RELEASE(m_pUIZBuff);
	SAFE_RELEASE(m_pZBuff);
	SAFE_RELEASE(m_pBackBufferZBuffer);

	// �T�[�t�F�C�X
	SAFE_RELEASE_ARRAY(m_pAfterimageRender);
	SAFE_RELEASE(m_pBackGroundRender);
	SAFE_RELEASE(m_pCharRender);
	SAFE_RELEASE(m_pEffectRender);
	SAFE_RELEASE(m_pUIRender);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pBackBuffer);

	// �e�N�X�`��
	SAFE_RELEASE_ARRAY(m_pAfterimageTexture);
	SAFE_RELEASE(m_pBackGroundTexture);
	SAFE_RELEASE(m_pCharTexture);
	SAFE_RELEASE(m_pEffectTexture);
	SAFE_RELEASE(m_pUITexture);
	SAFE_RELEASE(m_pTexture);

	SAFE_RELEASE(m_pD3DDevice); // �f�o�C�X
	SAFE_RELEASE(m_pD3D);       // �I�u�W�F�N�g
}

//------------------------------
// �X�V����
//------------------------------
void CRenderer::Update(void) const
{
	CManager::GetGui().Update(); // Gui�X�V

	if (CManager::IsPause())
	{

	}
	else
	{
		CObject::UpdateAll();        // �I�u�W�F�N�g�̍X�V
	}
}

//------------------------------
//�`�揈��
//------------------------------
HRESULT CRenderer::Draw(void)
{
	//-----------------------------
	// �w�i
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::BackGround))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		SetRender(); // �`��ݒ�

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(0); // �I�u�W�F�N�g�̕`��
		CObject::DrawPriority(1); // �I�u�W�F�N�g�̕`��
		m_pD3DDevice->EndScene(); // �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
    // �c��
    //-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Afterimage))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		RenderTexture(m_pAfterimageTexture[1], White(0.95f), false, false, true);

		SetRender(); // �`��ݒ�
		CManager::GetCamera()->Set(0);  // �J����
		CObject::DrawPriority(3, true); // �c���I�u�W�F�N�g�̕`��
		m_pD3DDevice->EndScene();       // �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
	// �L�����N�^�[
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Char))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		SetRender(); // �`��ݒ�

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(2); // �I�u�W�F�N�g�̕`��
		RenderTexture(m_pAfterimageTexture[0], WHITE, false, true, true, true); // �c��
		CObject::DrawPriority(3); // �I�u�W�F�N�g�̕`��
		CObject::DrawPriority(4); // �I�u�W�F�N�g�̕`��
		m_pD3DDevice->EndScene(); // �`��I��
	}
	else
	{
		return E_FAIL;
	}
	//-----------------------------
	// �G�t�F�N�g
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Effect))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		SetRender(); // �`��ݒ�

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(5); // �I�u�W�F�N�g�̕`��
		CObject::DrawPriority(6); // �I�u�W�F�N�g�̕`��
		m_pD3DDevice->EndScene(); // �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
	// UI
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::UI))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		SetRender(); // �`��ݒ�
		CObject::DrawPriority(7);  // �I�u�W�F�N�g�̕`��
		m_pD3DDevice->EndScene();  // �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//----------------------------------------
    // ���C�� (���킹��1�̃e�N�X�`���ɂ���)
    //----------------------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Main))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		RenderTexture(m_pBackGroundTexture, WHITE, false, false, true, true);   // �w�i
		RenderTexture(m_pCharTexture, WHITE, false, true, true, true);          // �L�����N�^�[
		RenderTexture(m_pEffectTexture, WHITE, false, true, true, true);        // �G�t�F�N�g
		RenderTexture(m_pUITexture, WHITE, false, true, true, true);            // UI
		m_pD3DDevice->EndScene();// �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//------------------------------------------
    // �o�b�N�o�b�t�@�ɂł����e�N�X�`��+����`��
    //------------------------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::BackBuffer))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//�`��J�n
	{
		RenderTexture(m_pTexture, WHITE, false, false, true, true);

		SetRender();               // �`��ݒ�
		CManager::GetGui().Draw(); // Gui�`��
		CDebugProc::Draw();        // �f�o�b�N�\��
		m_pD3DDevice->EndScene();  // �`��I��
	}
	else
	{
		return E_FAIL;
	}

	//�o�b�N�o�b�t�@�ɕ\����؂�ւ���
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);

	// ����ւ�
	swap(m_pAfterimageTexture[0], m_pAfterimageTexture[1]);
	swap(m_pAfterimageRender[0], m_pAfterimageRender[1]);

	return S_OK;
}

//------------------------------
// ���_�ݒ�
//------------------------------
HRESULT CRenderer::SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, span<const DirectX2D::Vertex> vertex, DirectX::Vector3 offSet, float angle)
{
	void* pVtx{};
	if (FAILED((*ppVtxBuffer)->Lock(0, 0, &pVtx, 0))) { return E_FAIL; }
	memcpy(pVtx, vertex.data(), sizeof(DirectX2D::Vertex) * vertex.size());
	if (FAILED((*ppVtxBuffer)->Unlock())) { return E_FAIL; }
	return S_OK;
}

//------------------------------
// ���_�ݒ�
//------------------------------
HRESULT CRenderer::SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, DirectX::Vector2 size, DirectX::Vector3 pos, float angle, DirectX::Color color, DirectX::Vector2 texBlock)
{
	vector<DirectX2D::Vertex> vertex{};
	void* pVtx{};
	if (FAILED((*ppVtxBuffer)->Lock(0, 0, &pVtx, 0))) { return E_FAIL; }
	memcpy(pVtx, vertex.data(), sizeof(DirectX2D::Vertex) * vertex.size());
	if (FAILED((*ppVtxBuffer)->Unlock())) { return E_FAIL; }
	return S_OK;
}

//------------------------------
// �`��ݒ�
//------------------------------
void CRenderer::SetRender(void) const
{
    // �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			 // �J�����O�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);     // �V�F�[�f�B���O���[�h���O�[���[�V�F�[�h�ɐݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                   // Z�o�b�t�@��L����
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              // Z�o�b�t�@�������݂�L����
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);         // Z�o�b�t�@����O�D��ɂ���
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			 // �v���C���[�̒��ɓ����x��������
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // �ʏ�̃u�����h
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // �ʏ�̃u�����h
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);          // �A���t�@�e�X�g
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       // ����
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);                     // 臒l
	m_pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);     // ���̃A���`�G�C���A�X (�K�C�h���̈���)
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);            // �X�e���V���o�b�t�@�I�t
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                  // ���C�g

	SetFog(true, D3DFOG_LINEAR, TRANSLUCENT_WHITE, 1000.0f, 1500.0f); // �t�H�O�̐ݒ�

	// GPU �ɍ��킹���ٕ����t�B���^�ݒ�
	D3DCAPS9 caps;
	m_pD3DDevice->GetDeviceCaps(&caps);
	DWORD maxAniso = min(caps.MaxAnisotropy, static_cast<DWORD>(16));

	// �T���u���[�X�e�[�g�̐ݒ�
	for (size_t cntTex = 0; cntTex < 1; cntTex++)
	{
		// �t�B���^�[�ݒ�
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAXANISOTROPY, maxAniso);        // �ٕ����t�B���^
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); // �ٕ����t�B���^
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC); // �ٕ����t�B���^

		// �~�b�v�}�b�v�̎g�p�i�����ɉ������œK�ȃe�N�X�`���I���j
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAXMIPLEVEL, 0);                  // 0���ł����𑜓x
		float lodBias = -0.75f;
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&lodBias); // LOD�o�C�A�X����
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);   // �~�b�v�}�b�v�⊮

		//�e�N�X�`���J��Ԃ�
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	// �e�N�X�`���X�e�[�W�̐ݒ�
	for (size_t cntTex = 0; cntTex < 1; cntTex++)
	{
		//�J���[
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);   // �|���Z
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   // �e�N�X�`���̐F��
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);   // ����
		//�����x
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);   // �|���Z
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);   // �e�N�X�`���̓����x��
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);    // ����
		//���̑�
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);                       //�e�N�X�`���̃C���f�b�N�X
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE); // �e�N�X�`���ϊ�
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);               // �e�N�X�`���X�e�[�W�̌���
	}

	// �X�e�[�W 1 �ȍ~�𖳌���
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE); // ��{��1��
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE); // ��{��1��
}

//----------------------------
// �t�H�O�̐ݒ�
//----------------------------
void CRenderer::SetFog(const bool bFog, D3DFOGMODE mode, D3DCOLOR color, const float start, const float end) const
{
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, bFog); // �t�H�O�̎g�p

	if (bFog)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, color);            // �t�H�O�̐F
		m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, mode);         // �t�H�O�̎��
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, BIT_DWORD(start)); // �t�H�O�̊J�n����
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, BIT_DWORD(end));     // �t�H�O�̏I������
	}
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(0, 0, 0, 0)); // �t�H�O�̐F���N���A
	}
}

//----------------------------
// �o�b�N�o�b�t�@�T�C�Y�̎擾
//----------------------------
HRESULT CRenderer::GetBackBufferSize(D3DXVECTOR2* size) const
{
	// DirectX�̃T�C�Y���擾����
	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	if (SUCCEEDED(m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
	{// �o�b�N�o�b�t�@�̎擾
		D3DSURFACE_DESC desc;
		pBackBuffer->GetDesc(&desc);
		size->x = (float)desc.Width;
		size->y = (float)desc.Height;
		pBackBuffer->Release();  // �擾��������
		pBackBuffer = nullptr;

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// �o�b�N�o�b�t�@�T�C�Y�ƃN���C�A���g�T�C�Y�̔䗦���擾
//---------------------------------------------------
HRESULT CRenderer::GetBackBufferToClientRatio(D3DXVECTOR2* ratio) const
{
	// DirectX�̃T�C�Y���擾����
	D3DXVECTOR2 dxSize;
	if (FAILED(GetBackBufferSize(&dxSize))) return E_FAIL;

	// �N���C�A���g�T�C�Y���擾����
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // �N���C�A���g�T�C�Y�̎擾

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = dxSize.x / clientSize.x;
	ratio->y = dxSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// �����_�[�^�[�Q�b�g�T�C�Y�̎擾
//----------------------------
HRESULT CRenderer::GetTargetSize(D3DXVECTOR2* size) const
{
	// DirectX�̃T�C�Y���擾����
	LPDIRECT3DSURFACE9 pTarget = nullptr;
	if (SUCCEEDED(m_pD3DDevice->GetRenderTarget(0, &pTarget)))
	{// �o�b�N�o�b�t�@�̎擾
		D3DSURFACE_DESC desc;
		pTarget->GetDesc(&desc);
		size->x = (float)desc.Width;
		size->y = (float)desc.Height;
		pTarget->Release();  // �擾��������
		pTarget = nullptr;

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// �����_�[�^�[�Q�b�g�T�C�Y�ƃN���C�A���g�T�C�Y�̔䗦���擾
//---------------------------------------------------
HRESULT CRenderer::GetTargetToClientRatio(D3DXVECTOR2* ratio) const
{
	// DirectX�̃T�C�Y���擾����
	D3DXVECTOR2 dxSize;
	if (FAILED(GetTargetSize(&dxSize))) return E_FAIL;

	// �N���C�A���g�T�C�Y���擾����
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // �N���C�A���g�T�C�Y�̎擾

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = dxSize.x / clientSize.x;
	ratio->y = dxSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// �r���[�|�[�g�T�C�Y�̎擾
//----------------------------
HRESULT CRenderer::GetViewportSize(D3DXVECTOR2* size) const
{
	// DirectX�̃T�C�Y���擾����
	D3DVIEWPORT9 viewport{};
	if (SUCCEEDED(m_pD3DDevice->GetViewport(&viewport)))
	{// �o�b�N�o�b�t�@�̎擾
		size->x = (float)viewport.Width;
		size->y = (float)viewport.Height;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// �r���[�|�[�g�ƃN���C�A���g�T�C�Y�̔䗦���擾
//---------------------------------------------------
HRESULT CRenderer::GetViewportToClientRatio(D3DXVECTOR2* ratio) const
{
	// �r���[�|�[�g�̃T�C�Y���擾����
	D3DXVECTOR2 viewportSize;
	if (FAILED(GetViewportSize(&viewportSize))) return E_FAIL;

	// �N���C�A���g�T�C�Y���擾����
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // �N���C�A���g�T�C�Y�̎擾

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = viewportSize.x / clientSize.x;
	ratio->y = viewportSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// �X�N���[���T�C�Y�ύX
//----------------------------
HRESULT CRenderer::ReSize(const BOOL bWindow)
{
	if (m_pD3DDevice==nullptr)
	{
		return E_FAIL;
	}

	Sleep:

	// �f�o�C�X�̏�Ԃ��m�F
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DRIVERINTERNALERROR)
	{// �h���C�o�G���[(�v���W�F�N�g���s�s�\)
		return hr;
	}
	else if (hr == D3DERR_DEVICELOST)
	{// �f�o�C�X���X�g (Reset�҂�)
		Sleep(60); // 60ms�ҋ@
		goto Sleep; // �ēx�m�F
	}
	else if (hr == D3DERR_DEVICENOTRESET || hr == S_OK)
	{// �f�o�C�X���X�g(Reset�\)���͐���
		if (FAILED(ChangeTarget(RENDER_TARGET::BackBuffer))) { return E_FAIL; }       // �o�b�N�o�b�t�@�ɖ߂��Ă���

		// �E�B���h�E���[�h�̐ݒ�
		if (m_d3dpp.Windowed != bWindow)
		{
			m_d3dpp.Windowed = bWindow;

			//�f�B�X�v���C���[�h
			D3DDISPLAYMODE d3ddm = {};//�_�C���N�gX�f�B�X�v���C���[�h�̕ϐ��錾
			if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			{
				return E_FAIL;
			}

			m_d3dpp.FullScreen_RefreshRateInHz = m_d3dpp.Windowed ? 0 : d3ddm.RefreshRate;
		}

		//�o�b�N�o�b�t�@�T�C�Y�̕ύX
		if (m_d3dpp.Windowed)
		{// �E�B���h�E���[�h
			m_d3dpp.BackBufferWidth = (UINT)CMain::SCREEN_WIDTH;
			m_d3dpp.BackBufferHeight = (UINT)CMain::SCREEN_HEIGHT;
		}
		else
		{// �t���X�N���[�����[�h
			RECT clientRect;
			if (FAILED(CMain::GetClientRect(&clientRect)))return E_FAIL;
			m_d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
			m_d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
		}

		DWORD qualityLevels = 0;
		const D3DMULTISAMPLE_TYPE samples[] =
		{
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
			D3DMULTISAMPLE_NONE
		};

		for (auto sample : samples)
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType
			(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				m_d3dpp.BackBufferFormat, m_d3dpp.Windowed,
				sample, &qualityLevels))
				)
			{
				m_d3dpp.MultiSampleType = sample;
				m_d3dpp.MultiSampleQuality = (sample == D3DMULTISAMPLE_NONE) ? 0 : qualityLevels - 1;
				break;
			}
		}

		// ���Z�b�g�O�̔j��
		ResetRelease();

		// �f�o�C�X�̃��Z�b�g
		if (FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
		{
			return E_FAIL;
		}

		// �f�o�C�X�̏�Ԃ��m�F
		if (FAILED(m_pD3DDevice->TestCooperativeLevel()))
		{
			return E_FAIL;
		}

		// �f�o�C�X�̃��Z�b�g��ɕ`��ݒ���ēx�s��
		SetRender();

		// ���Z�b�g��̍Đ���
		ResetCreate();

		return S_OK;
	}
	else
	{// ���̑��̃G���[
		return hr;
	}
}

//----------------------------
// �J�[�\���ʒu�̎擾
//----------------------------
D3DXVECTOR2 CRenderer::GetCursorPos(void) const
{
	POINT cursorPint = CMain::GetCursorPoint();

	// �N���C�A���g���W�ƃ����_�[�^�[�Q�b�g�T�C�Y�̔䗦
	D3DXVECTOR2 ratio{};
	if (FAILED(GetTargetToClientRatio(&ratio))) return D3DXVECTOR2(~0, ~0);

	// �����_�[�^�[�Q�b�g���ł̈ʒu
	D3DXVECTOR2 cursorPos{};
	cursorPos.x = static_cast<float>(cursorPint.x) * ratio.x;
	cursorPos.y = static_cast<float>(cursorPint.y) * ratio.y;

	// �����_�[�^�[�Q�b�g�T�C�Y
	D3DXVECTOR2 screenSize{};
	if (FAILED(GetTargetSize(&screenSize))) return D3DXVECTOR2(~0, ~0);

	// �����_�[�^�[�Q�b�g���ł̈ʒu�̊���
	cursorPos.x /= screenSize.x;
	cursorPos.y /= screenSize.y;

	return cursorPos;
}

//----------------------------
// �J�[�\���ʒu�̎擾
//----------------------------
HRESULT CRenderer::GetCursorPos(D3DXVECTOR2* pOutpos) const
{
	POINT cursorPint = CMain::GetCursorPoint();

	// �N���C�A���g���W�ƃ����_�[�^�[�Q�b�g�T�C�Y�̔䗦
	D3DXVECTOR2 ratio{};
	if (FAILED(GetTargetToClientRatio(&ratio))) return E_FAIL;

	// �����_�[�^�[�Q�b�g���ł̈ʒu
	D3DXVECTOR2 cursorPos{};
	pOutpos->x = static_cast<float>(cursorPint.x) * ratio.x;
	pOutpos->y = static_cast<float>(cursorPint.y) * ratio.y;

	// �����_�[�^�[�Q�b�g�T�C�Y
	D3DXVECTOR2 screenSize{};
	if (FAILED(GetTargetSize(&screenSize))) return E_FAIL;

	// �����_�[�^�[�Q�b�g���ł̈ʒu�̊���
	pOutpos->x /= screenSize.x;
	pOutpos->y /= screenSize.y;

	return S_OK;
}

//----------------------------
// �����_�[�^�[�Q�b�g�̕ύX
//----------------------------
HRESULT CRenderer::ChangeTarget(RENDER_TARGET target)
{
	// �����_�[�^�[�Q�b�g
	switch (target)
	{
	case RENDER_TARGET::BackBuffer:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pBackBuffer))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pBackBufferZBuffer))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_backBufferViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Main:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_viewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::BackGround:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pBackGroundRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pBackGroundZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_backGroundViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Char:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pCharRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pCharZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_charViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Effect:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pEffectRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pEffectZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_effectViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::UI:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pUIRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pUIZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_UIViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Afterimage:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pAfterimageRender[0]))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pAfterimageZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_afterimageViewport))) { return E_FAIL; }
		break;
	}

	return S_OK;
}

//----------------------------
// �����_�[�e�N�X�`���̎擾
//----------------------------
LPDIRECT3DTEXTURE9 CRenderer::GetTexture(void) const
{
	return m_pTexture;
}

//----------------------------
// ���Z�b�g�O�̔j��
//----------------------------
void CRenderer::ResetRelease(void)
{
	// ���_�o�b�t�@
	SAFE_RELEASE(m_pVtx);

	// Z�o�b�t�@
	SAFE_RELEASE(m_pAfterimageZBuff);
	SAFE_RELEASE(m_pBackGroundZBuff);
	SAFE_RELEASE(m_pCharZBuff);
	SAFE_RELEASE(m_pEffectZBuff);
	SAFE_RELEASE(m_pUIZBuff);
	SAFE_RELEASE(m_pZBuff);
	SAFE_RELEASE(m_pBackBufferZBuffer);

	// �T�[�t�F�C�X
	SAFE_RELEASE_ARRAY(m_pAfterimageRender);
	SAFE_RELEASE(m_pBackGroundRender);
	SAFE_RELEASE(m_pCharRender);
	SAFE_RELEASE(m_pEffectRender);
	SAFE_RELEASE(m_pUIRender);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pBackBuffer);

	// �e�N�X�`��
	SAFE_RELEASE_ARRAY(m_pAfterimageTexture);
	SAFE_RELEASE(m_pBackGroundTexture);
	SAFE_RELEASE(m_pCharTexture);
	SAFE_RELEASE(m_pEffectTexture);
	SAFE_RELEASE(m_pUITexture);
	SAFE_RELEASE(m_pTexture);

	CDebugProc::Uninit();                  // �f�o�b�N�\��
	CManager::GetGui().InvalidateDevice(); // Gui
	CManager::GetLight()->Uninit();        // ���C�g
}

//----------------------------
// ���Z�b�g��̍Đ���
//----------------------------
HRESULT CRenderer::ResetCreate(void)
{
	CManager::GetLight()->Init();                      // ���C�g
	CManager::GetGui().CreateDevice();                 // Gui
	if (FAILED(CDebugProc::Init())) { return E_FAIL; } // �f�o�b�N�\��
	if (FAILED(SetTexture())) { return E_FAIL; } 	   // �e�N�X�`���`��

	return S_OK;
}

//--------------------------------
// �e�N�X�`����Viewport�S�̂ɒ���
//--------------------------------
HRESULT CRenderer::RenderTexture(LPDIRECT3DTEXTURE9 pTexture, D3DCOLOR color, bool blendOne, bool zEnable, bool bAlphaTest, bool bPoint)
{
	if (pTexture == nullptr) return S_OK; // �e�N�X�`����null�̎��͉����`�悵�Ȃ��̂�������

	Vector2 size{};
	GetViewportSize(&size);
	array<DirectX2D::Vertex, 4> vertex;
	for (Index8 cntVtx = 0; cntVtx < 4; cntVtx++)
	{
		vertex[cntVtx].pos = D3DXVECTOR3(size.x * float(cntVtx % 2), size.y * float(cntVtx / 2), 0.0f);
		vertex[cntVtx].col = color;
		vertex[cntVtx].tex = D3DXVECTOR2(float(cntVtx % 2), float(cntVtx / 2));
	}

	SetVertex2D(&m_pVtx, vertex);

	// ���_�o�b�t�@
	m_pD3DDevice->SetStreamSource(0, m_pVtx, 0, sizeof(DirectX2D::Vertex));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(DirectX2D::FVF_VERTEX);

	m_pD3DDevice->SetTexture(0, pTexture);

	SetRender(); // �ʏ�`��
	// �c�������F���Z�u�����h
	if (blendOne)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, zEnable);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, bAlphaTest);
	if (bPoint)
	{
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}
	// �|���S���̕`��
	HRESULT hr = m_pD3DDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//�^�C�v
		0,                  // �C���f�b�N�X
		2                   //�|���S����
	);
	SetRender(); // ���ɖ߂�
	return hr;
}

//----------------------------
// ���Z�b�g��̍Đ���
//----------------------------
HRESULT CRenderer::SetTexture(void)
{
	// �o�b�N�o�b�t�@�̃T�C�Y
	Vector2 backBufferSize{};
	GetBackBufferSize(&backBufferSize);

	// �`��p�|���S���̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer
	(
		sizeof(DirectX2D::Vertex) * VT_DEF,
		D3DUSAGE_WRITEONLY,
		DirectX2D::FVF_VERTEX,
		D3DPOOL_DEFAULT,
		&m_pVtx,
		nullptr
	)))
	{
		return E_FAIL;
	}

	// �`��p�|���S���ݒ�
	array<DirectX2D::Vertex, 4> vertex;
	for (Index8 cntVtx = 0; cntVtx < 4; cntVtx++)
	{
		vertex[cntVtx].pos = D3DXVECTOR3(backBufferSize.x * float(cntVtx % 2), backBufferSize.y * float(cntVtx / 2), 0.0f);
		vertex[cntVtx].tex = D3DXVECTOR2(float(cntVtx % 2), float(cntVtx / 2));
	}
	SetVertex2D(&m_pVtx, vertex);

	// �o�b�N�o�b�t�@�����擾����
	if (FAILED(m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))) return E_FAIL;
	if (FAILED(m_pD3DDevice->GetDepthStencilSurface(&m_pBackBufferZBuffer))) return E_FAIL;
	if (FAILED(m_pD3DDevice->GetViewport(&m_backBufferViewport))) return E_FAIL;

	m_pD3DDevice->ColorFill(m_pBackBuffer, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	//-----------------
	// ���C���e�N�X�`��
	//-----------------
	// texture����
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface�擾
	m_pTexture->GetSurfaceLevel(0, &m_pRender);
	m_pD3DDevice->ColorFill(m_pRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_viewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	//-----------------
	// �w�i�`��e�N�X�`��
	//-----------------
	// texture����
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBackGroundTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface�擾
	m_pBackGroundTexture->GetSurfaceLevel(0, &m_pBackGroundRender);
	m_pD3DDevice->ColorFill(m_pBackGroundRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBackGroundZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_backGroundViewport.X = 0;
	m_backGroundViewport.Y = 0;
	m_backGroundViewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_backGroundViewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_backGroundViewport.MinZ = 0.0f;
	m_backGroundViewport.MaxZ = 1.0f;

	//-----------------
    // �L�����N�^�[�`��e�N�X�`��
    //-----------------
    // texture����
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pCharTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface�擾
	m_pCharTexture->GetSurfaceLevel(0, &m_pCharRender);
	m_pD3DDevice->ColorFill(m_pCharRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pCharZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_charViewport.X = 0;
	m_charViewport.Y = 0;
	m_charViewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_charViewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_charViewport.MinZ = 0.0f;
	m_charViewport.MaxZ = 1.0f;

	//-----------------
    // �G�t�F�N�g�`��e�N�X�`��
    //-----------------
    // texture����
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pEffectTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface�擾
	m_pEffectTexture->GetSurfaceLevel(0, &m_pEffectRender);
	m_pD3DDevice->ColorFill(m_pEffectRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pEffectZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_effectViewport.X = 0;
	m_effectViewport.Y = 0;
	m_effectViewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_effectViewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_effectViewport.MinZ = 0.0f;
	m_effectViewport.MaxZ = 1.0f;

	//-----------------
    // UI�`��e�N�X�`��
    //-----------------
    // texture����
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pUITexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface�擾
	m_pUITexture->GetSurfaceLevel(0, &m_pUIRender);
	m_pD3DDevice->ColorFill(m_pUIRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pUIZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_UIViewport.X = 0;
	m_UIViewport.Y = 0;
	m_UIViewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_UIViewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_UIViewport.MinZ = 0.0f;
	m_UIViewport.MaxZ = 1.0f;

	//-----------------
	// �c���`��e�N�X�`��
	//-----------------
	for (Index8 cnt = 0; cnt < m_pAfterimageTexture.size(); ++cnt)
	{
		// texture����
		if (FAILED(m_pD3DDevice->CreateTexture
		(
			UINT(backBufferSize.x), UINT(backBufferSize.y),
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pAfterimageTexture[cnt], nullptr
		)))
		{
			return E_FAIL;
		}

		// Surface�擾
		m_pAfterimageTexture[cnt]->GetSurfaceLevel(0, &m_pAfterimageRender[cnt]);

		m_pD3DDevice->ColorFill(m_pAfterimageRender[cnt], nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));
	}

	// Z�o�b�t�@����
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pAfterimageZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// �r���[�|�[�g�̊e�p�����[�^�������Őݒ肷��
	m_afterimageViewport.X = 0;
	m_afterimageViewport.Y = 0;
	m_afterimageViewport.Width = DWORD(backBufferSize.x);  // �e�N�X�`���̕�
	m_afterimageViewport.Height = DWORD(backBufferSize.y); // �e�N�X�`���̍���
	m_afterimageViewport.MinZ = 0.0f;
	m_afterimageViewport.MaxZ = 1.0f;

	return S_OK;
}