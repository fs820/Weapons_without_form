//------------------------------------------
//
//�`��p�̏���[object.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "object.h"
#include "manager.h"

using namespace DirectX; // DirectX���O��Ԃ̎g�p

//---------------------------------------
//
// �I�u�W�F�N�g��{�N���X
//
//---------------------------------------
array<vector<CObject*>, 8> CObject::m_apObject{};   // �I�u�W�F�N�g�̃|�C���^�z��
int CObject::m_nAll = 0;

//------------------------------
// �S�X�V
//------------------------------
void CObject::UpdateAll(const context::UpdateContext updateContext)
{
	CallCollision(); // �Փ˔�����Ăяo��

	for (auto& priority : m_apObject)
	{// priority����
		for (Index cntObject = 0; cntObject < priority.size();)
		{// priority������
			if (priority[cntObject] != nullptr)
			{// �I�u�W�F�N�g������
				priority[cntObject]->Update(); // �I�u�W�F�N�g�̍X�V

				if (priority[cntObject]->IsRelease())
				{// �폜
					Release(priority[cntObject]); // �I�u�W�F�N�g��j��
					continue;                     // �폜���̓J�E���g�������Ȃ�
				}
			}
			else
			{// �I�u�W�F�N�g���Ȃ�
				do
				{// null�łȂ��v�f������܂�
					SwapRemove(priority, cntObject); // �I�u�W�F�N�g�|�C���^��z�񂩂珜��
					if (priority[cntObject] != nullptr)
					{// ���݂���
						priority[cntObject]->m_ID = cntObject; // �C���f�b�N�X�X�V
						break;
					}
				} while (true);
			}
			++cntObject; // �J�E���g�i�߂�
		}
	}
}

//------------------------------
// �S�`��
//------------------------------
void CObject::DrawAll(const context::DrawContext drawContext, bool bAfterimage)
{
	for (auto& priority : m_apObject)
	{// priority�z��
		for (auto& pObject : priority)
		{// �I�u�W�F�N�g�z��
			if (pObject != nullptr && (!bAfterimage || pObject->IsAfterimage()))
			{// ���݂���
				pObject->Draw();
			}
		}
	}
}

//------------------------------
// �v���C�I���e�B�`��
//------------------------------
void CObject::DrawPriority(Index priority, const context::DrawContext drawContext, bool bAfterimage)
{
	if (priority < m_apObject.size())
	{
		for (auto& pObject : m_apObject[priority])
		{// �I�u�W�F�N�g�z��
			if (pObject != nullptr && (!bAfterimage || pObject->IsAfterimage()))
			{// ���݂���
				pObject->Draw();
			}
		}
	}
}

//------------------------------
// �S�j��
//------------------------------
void CObject::ReleaseAll(void)
{
	for (auto& priority : m_apObject)
	{// priority�z��
		for (auto& pObject : priority)
		{// �I�u�W�F�N�g�z��
			SAFE_UNINIT(pObject); // �I�u�W�F�N�g�̃A���C�j�b�g���Ăяo��
		}
	}
	m_apObject.fill(vector<CObject*>()); // �S�ẴI�u�W�F�N�g�|�C���^���N���A
	m_nAll = 0;                          // �S�I�u�W�F�N�g�������Z�b�g
}

//------------------------------
// ������
//------------------------------
void CObject::Init(TYPE type)
{
	// �I�u�W�F�N�g�z��Ɏ�����o�^����
	m_apObject[m_Priority].push_back(this);
	m_ID = m_apObject[m_Priority].size() - 1;
	m_nAll++;

	m_transform = {}; // �g�����X�t�H�[��������
	m_type = type;    // �I�u�W�F�N�g�̎�ނ�ݒ�
}

//------------------------------
// �j��
//------------------------------
void CObject::Release(CObject* pObject)
{
	if (pObject == nullptr) return;                                 // null�`�F�b�N
	Index priority = pObject->GetPriority(), id = pObject->GetID(); // �z�񂩂�폜���邽�߂̏����폜�O�ɂ��炤
	SAFE_UNINIT(pObject);                                           // �I�u�W�F�N�g���폜
	do
	{// null�łȂ��v�f������܂�
		SwapRemove(m_apObject[priority], id); // �I�u�W�F�N�g�|�C���^��z�񂩂珜��
		if (m_apObject[priority][id] != nullptr)
		{// ���݂���
			m_apObject[priority][id]->m_ID = id; // �C���f�b�N�X�X�V
			break;
		}
	} while (true);
	--m_nAll; // �I�u�W�F�N�g�������炷
}

//------------------------------
// OnCollosion�Ăяo��
//------------------------------
void CObject::CallCollision(void)
{
	for (Index hostPri = 0; hostPri < m_apObject.size(); ++hostPri)
	{
		auto& hostVec = m_apObject[hostPri];
		for (Index hostIdx = 0; hostIdx < hostVec.size(); ++hostIdx)
		{
			CObject* pHost = hostVec[hostIdx];
			if (pHost == nullptr || !pHost->m_bCollision || pHost->m_type == TYPE::None) continue;

			// �Q�X�g�̓z�X�g�̒��ォ��S�̂���
			for (Index guestPri = hostPri; guestPri < m_apObject.size(); ++guestPri)
			{
				auto& guestVec = m_apObject[guestPri];

				// guestPri == hostPri �̂Ƃ���guestIdx��hostIdx + 1����J�n
				Index startGuestIdx = (guestPri == hostPri) ? hostIdx + 1 : 0;

				for (Index guestIdx = startGuestIdx; guestIdx < guestVec.size(); ++guestIdx)
				{
					CObject* pGuest = guestVec[guestIdx];
					if (pGuest == nullptr || !pGuest->m_bCollision || pGuest->m_type == TYPE::None) continue;

					if (CallCollisionHelper(*pHost, *pGuest))
					{
						pHost->OnCollision(*pGuest);

						// pHost��OnCollision�Ŕj������Ă��Ȃ����m�F�inullptr�`�F�b�N�j
						if (pHost != nullptr)
						{
							pGuest->OnCollision(*pHost);
						}
					}
				}
			}
		}
	}
}

//------------------------------
// �Փ˔���w���p�[�֐�
//------------------------------
bool CObject::CallCollisionHelper(const CObject& HostObject, const CObject& GuestObject)
{
	//------------------------------------------------------
	// �ЂƂ܂��~�̓����蔻��ōs�� (�K�v�ɉ����ċ�`����)
	//------------------------------------------------------

	// �g�����X�t�H�[�����T�C�Y���擾
	Transform HostTrans = HostObject.GetTransform();
	D3DXVECTOR3 HostSize = HostObject.GetSize();
	HostSize.x *= HostTrans.scale.x;
	HostSize.y *= HostTrans.scale.y;
	HostSize.z *= HostTrans.scale.z;
	Transform GuestTrans = GuestObject.GetTransform();
	D3DXVECTOR3 GuestSize = GuestObject.GetSize();
	GuestSize.x *= GuestTrans.scale.x;
	GuestSize.z *= GuestTrans.scale.z;

	// ���a
	float HostLength = D3DXVec3Length(&HostSize);
	float GuestLength = D3DXVec3Length(&GuestSize);

	//// �X�N���[�����W�ɕϊ�
	//D3DXVECTOR2 screensize = {};
	//CManager::GetRenderer().GetDxScreenSize(&screensize);
	//HostTrans.pos.x *= screensize.x;
	//HostTrans.pos.y *= screensize.y;
	//GuestTrans.pos.x *= screensize.x;
	//GuestTrans.pos.y *= screensize.y;

	D3DXVECTOR3 Space = GuestTrans.pos - HostTrans.pos; // �z�X�g�ƃQ�X�g�̈ʒu�x�N�g�����v�Z
	if (D3DXVec3Length(&Space) < (HostLength + GuestLength) * 0.5f)
	{// �~���ڂ��Ă���
		return true;
	}
	return false;
}