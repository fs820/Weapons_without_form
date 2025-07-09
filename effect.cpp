//------------------------------------------
//
// Effect���� [effect.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "effect.h"
#include "renderer.h"
#include "manager.h"

using namespace DirectX3D; // DirectX3D��Ԃ̎g�p

//---------------------------------------
//
// Effect
//
//---------------------------------------

// �ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CEffect::m_apTexture = nullptr; // ���L�e�N�X�`���̃|�C���^
D3DXVECTOR2 CEffect::m_aImageSize = {};            // �e�N�X�`���T�C�Y

//------------------------------
// �\�[�X�ǂݍ���
//------------------------------
HRESULT CEffect::Load(const string_view sTexturePass)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// �e�N�X�`��
	D3DXIMAGE_INFO imageInfo = {};
	if (FAILED(D3DXGetImageInfoFromFile
	(
		sTexturePass.data(),
		&imageInfo
	)))
	{
		return E_FAIL;
	}
	m_aImageSize = D3DXVECTOR2((float)imageInfo.Width, (float)imageInfo.Height);

	if (FAILED(D3DXCreateTextureFromFile
	(
		pDevice,
		sTexturePass.data(),
		&m_apTexture
	)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------
// �\�[�X�j��
//------------------------------
void CEffect::Unload(void)
{
	// �e�N�X�`��
	if (m_apTexture != nullptr)
	{
		m_apTexture->Release();
		m_apTexture = nullptr;
	}
}

//------------------------------
// ����
//------------------------------
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int priority, D3DXCOLOR color, float fSpeed)
{
	CEffect* pEffect = new CEffect(priority); // �C���X�^���X����

	if (pEffect == nullptr)
	{// �������s
		return nullptr;
	}

	D3DXVECTOR3 imageSize = D3DXVECTOR3(m_aImageSize.x, m_aImageSize.y, 0.0f); // �e�N�X�`���T�C�Y�̎擾
	pEffect->SetSize(imageSize); // �e�N�X�`���T�C�Y�̐ݒ�

	// ������
	if (FAILED(pEffect->Init(pos, rot, scale, color, fSpeed)))
	{
		delete pEffect;
		pEffect = nullptr;
		return nullptr;
	}

	return pEffect;
}

//------------------------------
//����������
//------------------------------
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXCOLOR color, float fSpeed)
{
	CObjectBillboard::Init(pos, rot, scale, TYPE::Effect); // �e�̏�����

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(CManager::GetRenderer().GetViewportSize(&screenSize)))
	{
		return E_FAIL;
	}

	m_color = color;
	m_fSpeed = fSpeed;             // �ړ����x
	m_fLife = LIFE;                 // ���C�t

	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return E_FAIL; }

	Transform transform = GetTransform();
	m_DefaultScale = transform.scale;

	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // �T�C�Y�̎擾
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);

	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = m_color;
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//�I������
//------------------------------
void CEffect::Uninit(void)
{
	CObjectBillboard::Uninit(); // �e�̏I��
}

//------------------------------
//�X�V����
//------------------------------
void CEffect::Update(void)
{
	if (m_fLife <= 0.0f)
	{// ���C�t��0�ȉ��Ȃ�΍폜
		SetRelease(true);
		return;
	}

	CObjectBillboard::Update(); // �e�̍X�V

	float deltaTime = CMain::GetDeltaTimeGameSpeed();

	m_fLife -= deltaTime; // ���C�t�����炷

	CRenderer renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer.GetDevice(); // �����_���[����f�o�C�X���擾

	Transform transform = GetTransform(); // �ό`���̎擾
	transform.pos.x += m_fSpeed * sinf(transform.rot.z) * deltaTime;
	transform.pos.y += m_fSpeed * cosf(transform.rot.z) * deltaTime;
	transform.pos.z += 0.0f;

	transform.scale.x = m_DefaultScale.x * (m_fLife / LIFE);
	transform.scale.y = m_DefaultScale.y * (m_fLife / LIFE);
	transform.scale.z += 1.0f;

	SetTransform(transform);

	// �X�N���[���T�C�Y�̎擾
	D3DXVECTOR2 screenSize = {};
	if (FAILED(renderer.GetViewportSize(&screenSize)))
	{
		return;
	}

	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	Vertex* pVtx;
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0))) { return; }

	// ��]�֌W����y
	D3DXVECTOR3 screenPos = D3DXVECTOR3(transform.pos.x * screenSize.x, transform.pos.y * screenSize.y, 0.0f);

	D3DXVECTOR3 resultPos[VT_DEF] = {};
	D3DXVECTOR3 size = GetSize(); // �T�C�Y�̎擾
	size.x *= transform.scale.x;
	size.y *= transform.scale.y;

	resultPos[0] = D3DXVECTOR3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[1] = D3DXVECTOR3(size.x * 0.5f, -size.y * 0.5f, 0.0f);
	resultPos[2] = D3DXVECTOR3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	resultPos[3] = D3DXVECTOR3(size.x * 0.5f, size.y * 0.5f, 0.0f);

	// ���_���̐ݒ�
	for (size_t cntVtx = 0; cntVtx < VT_DEF; cntVtx++)
	{
		resultPos[cntVtx] += screenPos;

		pVtx[cntVtx].pos = resultPos[cntVtx];
		pVtx[cntVtx].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		pVtx[cntVtx].col = m_color;
		pVtx[cntVtx].tex = D3DXVECTOR2((float)(cntVtx % 2) * 1.0f, (float)(cntVtx / 2) * 1.0f);
	}

	if (FAILED(pVtxBuff->Unlock())) { return; }
}

//------------------------------
//�`�揈��
//------------------------------
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBillboard::Draw(); // �e�̕`��

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}