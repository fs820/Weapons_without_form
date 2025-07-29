//----------------------------------------------------
//
// ���[�V�������� [motion.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "motion.h"
#include "manager.h"
#include "model.h"

using namespace motion; // ���[�V������Ԃ̎g�p

//----------------------------------------------------
//
// ���f���N���X
//
//----------------------------------------------------

//----------------------------------------------------
// ���[�V�����Z�b�g
//----------------------------------------------------
void CMotion::Set(const Motion* pMotion, bool bBlend, int blendFrame)
{
	if (bBlend && m_pMotion != nullptr && m_pMotion->aKey.size() > 1)
	{// �u�����h����
		m_pBlendMotion = pMotion;
		m_data.bLoopBlend = pMotion->bLoop;	// ���[�v�̑��
		m_data.keyBlend = 0;				// �������Ă���L�[��������
		if (pMotion->aKey.size() > 1)
		{// ��L�[�ȏ゠��Ȃ�
			m_data.nextKeyBlend = m_data.keyBlend + 1; // ���̃L�[��ݒ�
		}
		else
		{// �Ȃ��Ȃ��
			m_data.nextKeyBlend = m_data.keyBlend;	// ���̃L�[��ݒ�
		}
		m_data.counterMotionBlend = 0;	// �t���[���J�E���^�[���[��
		m_data.counterKeyBlend = 0;	    // �L�[�J�E���^�[���[��
		m_data.MotionEndBlend = false;  // ���[�V�����͏I����Ă��Ȃ�

		m_data.blendFrame = blendFrame; // �u�����h�t���[��
		m_data.blendCounter = 0;        // �u�����h�J�E���^�[
		m_data.bBlend = true;           // �u�����h����
	}
	else
	{// �u�����h���Ȃ�
		m_pMotion = pMotion;
		m_data.bLoop = pMotion->bLoop;	// ���[�v�̑��
		m_data.key = 0;				    // �������Ă���L�[��������
		if (pMotion->aKey.size() > 1)
		{// ��L�[�ȏ゠��Ȃ�
			m_data.nextKey = m_data.key + 1;  // ���̃L�[��ݒ�
		}
		else
		{// �Ȃ��Ȃ��
			m_data.nextKey = m_data.key;	// ���̃L�[��ݒ�
		}
		m_data.counterMotion = 0;	// �t���[���J�E���^�[���[��
		m_data.counterKey = 0;		// �L�[�J�E���^�[���[��
		m_data.MotionEnd = false;   // ���[�V�����͏I����Ă��Ȃ�
		m_data.bBlend = false;      // �u�����h���Ȃ�

		//�u�����h�p�̕ϐ������������Ă���
		m_pBlendMotion = nullptr;
		m_data.bLoopBlend = false;	            // ���[�v�̑��
		m_data.keyBlend = INVALID_ID;			// �������Ă���L�[��������
		m_data.nextKeyBlend = INVALID_ID;       // ���̃L�[��ݒ�
		m_data.counterMotionBlend = INVALID_ID; // �t���[���J�E���^�[���[��
		m_data.counterKeyBlend = INVALID_ID;	// �L�[�J�E���^�[���[��
		m_data.MotionEndBlend = false;          // ���[�V�����͏I����Ă��Ȃ�

		m_data.blendFrame = 0;    // �u�����h�t���[��
		m_data.blendCounter = 0;  // �u�����h�J�E���^�[
	}
}

//----------------------------------------------------
// ���[�V�����̍X�V
//----------------------------------------------------
bool CMotion::Update(void)
{
	Index modelNum = m_modelIndex.size(); // �o�^����Ă��郂�f����
	if (modelNum > m_pMotion->aKey[0].aParts.size())
	{
		return false;
	}

	//���̃��[�V����
	vector<Parts> nextKey(modelNum); // ���̃L�[
	vector<Parts> nowKey(modelNum);  // ���̃L�[
	vector<Parts> diffKey(modelNum); // �L�[�̍���
	vector<Parts> Hope(modelNum);	 // ��]�̒l

	//�u�����h��̃��[�V����
	vector<Parts> nextKeyBlend(modelNum); // ���̃L�[
	vector<Parts> nowKeyBlend(modelNum);  // ���̃L�[
	vector<Parts> diffKeyBlend(modelNum); // �L�[�̍���
	vector<Parts> HopeBlend(modelNum);	  // ��]�̒l

	//����
	vector<Parts> diffKeyDest(modelNum); // �L�[�̍���
	vector<Parts> HopeDest(modelNum);	 // ��]�̒l

	if (m_data.nextKey < m_pMotion->aKey.size())
	{// ���[�V�����̑S�̃L�[���l���Ⴂ�ꍇ

		++m_data.counterMotion;	// ���[�V�����J�E���g
		++m_data.counterKey;		// �L�[�J�E���g

		// �S���f���p�[�c�̍X�V
		for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
		{
			nextKey[cntModel] = m_pMotion->aKey[m_data.nextKey].aParts[cntModel]; // ���̃L�[
			nowKey[cntModel] = m_pMotion->aKey[m_data.key].aParts[cntModel];      // ���̃L�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKey[cntModel].pos.x = (nextKey[cntModel].pos.x - nowKey[cntModel].pos.x);
			diffKey[cntModel].pos.y = (nextKey[cntModel].pos.y - nowKey[cntModel].pos.y);
			diffKey[cntModel].pos.z = (nextKey[cntModel].pos.z - nowKey[cntModel].pos.z);

			// �L�[�̍���(����)�����߂�
			diffKey[cntModel].rot.x = (nextKey[cntModel].rot.x - nowKey[cntModel].rot.x);
			diffKey[cntModel].rot.y = (nextKey[cntModel].rot.y - nowKey[cntModel].rot.y);
			diffKey[cntModel].rot.z = (nextKey[cntModel].rot.z - nowKey[cntModel].rot.z);

			// �I�t�Z�b�g�ɑ����l�����߂�
			Hope[cntModel].pos.x = nowKey[cntModel].pos.x + diffKey[cntModel].pos.x * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].pos.y = nowKey[cntModel].pos.y + diffKey[cntModel].pos.y * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].pos.z = nowKey[cntModel].pos.z + diffKey[cntModel].pos.z * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);

			// ���������߂�
			Hope[cntModel].rot.x = nowKey[cntModel].rot.x + diffKey[cntModel].rot.x * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].rot.y = nowKey[cntModel].rot.y + diffKey[cntModel].rot.y * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].rot.z = nowKey[cntModel].rot.z + diffKey[cntModel].rot.z * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
		}

		if (m_data.counterKey >= m_pMotion->aKey[m_data.key].frame)
		{// �t���[���S�������I�������
			m_data.counterKey = 0;          	        // �L�[�J�E���g�[��
			++m_data.nextKey;	                        // ���̃L�[�փJ�E���g
			++m_data.key;		                        // ���̃L�[�փJ�E���g
			m_data.nextKey %= m_pMotion->aKey.size();	// �L�[�̑������ɐ�������
			m_data.key %= m_pMotion->aKey.size();		// �L�[�̑������ɐ�������
			if (m_data.nextKey == 0 && m_data.bLoop == false)
			{// ���[�v���Ȃ����[�V�����͏I��点��
				m_data.nextKey = m_pMotion->aKey.size();	// �v�Z�̏������ɓ���Ȃ��悤�ɂ���
				m_data.counterMotion = 0;			        // ���[�V�����J�E���g�[��
				m_data.MotionEnd = true;		            // ���[�V�����I���
			}
		}
	}

	if (m_data.bBlend == true && m_data.nextKeyBlend < m_pBlendMotion->aKey.size())
	{// �u�����h���邩���[�V�����̑S�̃L�[���l���Ⴂ�ꍇ
		++m_data.counterMotionBlend;	// ���[�V�����J�E���g
		++m_data.counterKeyBlend;		// �L�[�J�E���g

		// �S���f���p�[�c�̍X�V
		for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
		{
			nextKeyBlend[cntModel] = m_pBlendMotion->aKey[m_data.nextKeyBlend].aParts[cntModel]; // ���̃L�[
			nowKeyBlend[cntModel] = m_pBlendMotion->aKey[m_data.keyBlend].aParts[cntModel];      // ���̃L�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKeyBlend[cntModel].pos.x = (nextKeyBlend[cntModel].pos.x - nowKeyBlend[cntModel].pos.x);
			diffKeyBlend[cntModel].pos.y = (nextKeyBlend[cntModel].pos.y - nowKeyBlend[cntModel].pos.y);
			diffKeyBlend[cntModel].pos.z = (nextKeyBlend[cntModel].pos.z - nowKeyBlend[cntModel].pos.z);

			// �L�[�̍���(����)�����߂�
			diffKeyBlend[cntModel].rot.x = (nextKeyBlend[cntModel].rot.x - nowKeyBlend[cntModel].rot.x);
			diffKeyBlend[cntModel].rot.y = (nextKeyBlend[cntModel].rot.y - nowKeyBlend[cntModel].rot.y);
			diffKeyBlend[cntModel].rot.z = (nextKeyBlend[cntModel].rot.z - nowKeyBlend[cntModel].rot.z);

			// �I�t�Z�b�g�ɑ����l�����߂�
			HopeBlend[cntModel].pos.x = nowKeyBlend[cntModel].pos.x + diffKeyBlend[cntModel].pos.x * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].pos.y = nowKeyBlend[cntModel].pos.y + diffKeyBlend[cntModel].pos.y * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].pos.z = nowKeyBlend[cntModel].pos.z + diffKeyBlend[cntModel].pos.z * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);

			// ���������߂�
			HopeBlend[cntModel].rot.x = nowKeyBlend[cntModel].rot.x + diffKeyBlend[cntModel].rot.x * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].rot.y = nowKeyBlend[cntModel].rot.y + diffKeyBlend[cntModel].rot.y * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].rot.z = nowKeyBlend[cntModel].rot.z + diffKeyBlend[cntModel].rot.z * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
		}

		if (m_data.counterKeyBlend >= m_pBlendMotion->aKey[m_data.keyBlend].frame)
		{// �t���[���S�������I�������
			m_data.counterKeyBlend = 0;          	        // �L�[�J�E���g�[��
			++m_data.nextKeyBlend;	                    // ���̃L�[�փJ�E���g
			++m_data.keyBlend;		                    // ���̃L�[�փJ�E���g
			m_data.nextKeyBlend %= m_pBlendMotion->aKey.size();	// �L�[�̑������ɐ�������
			m_data.keyBlend %= m_pBlendMotion->aKey.size();		// �L�[�̑������ɐ�������

			if (m_data.nextKeyBlend == 0 && m_data.bLoopBlend == false)
			{// ���[�v���Ȃ����[�V�����͏I��点��
				m_data.nextKeyBlend = m_pBlendMotion->aKey.size();	// �v�Z�̏������ɓ���Ȃ��悤�ɂ���
				m_data.counterMotionBlend = 0;			    // ���[�V�����J�E���g�[��
				m_data.MotionEndBlend = true;		            // ���[�V�����I���
			}
		}
	}

	// �S���f���p�[�c�̍X�V
	for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
	{
		if (m_data.bBlend == true)
		{
			++m_data.blendCounter; //�u�����h�J�E���^�[

			// �L�[�̍���(�ʒu)�����߂�
			diffKeyDest[cntModel].pos.x = (HopeBlend[cntModel].pos.x - Hope[cntModel].pos.x);
			diffKeyDest[cntModel].pos.y = (HopeBlend[cntModel].pos.y - Hope[cntModel].pos.y);
			diffKeyDest[cntModel].pos.z = (HopeBlend[cntModel].pos.z - Hope[cntModel].pos.z);

			// �L�[�̍���(����)�����߂�
			diffKeyDest[cntModel].rot.x = (HopeBlend[cntModel].rot.x - Hope[cntModel].rot.x);
			diffKeyDest[cntModel].rot.y = (HopeBlend[cntModel].rot.y - Hope[cntModel].rot.y);
			diffKeyDest[cntModel].rot.z = (HopeBlend[cntModel].rot.z - Hope[cntModel].rot.z);

			// �I�t�Z�b�g�ɑ����l�����߂�
			HopeDest[cntModel].pos.x = Hope[cntModel].pos.x + diffKeyDest[cntModel].pos.x * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].pos.y = Hope[cntModel].pos.y + diffKeyDest[cntModel].pos.y * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].pos.z = Hope[cntModel].pos.z + diffKeyDest[cntModel].pos.z * ((float)m_data.blendCounter / (float)m_data.blendFrame);

			// ���������߂�
			HopeDest[cntModel].rot.x = Hope[cntModel].rot.x + diffKeyDest[cntModel].rot.x * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].rot.y = Hope[cntModel].rot.y + diffKeyDest[cntModel].rot.y * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].rot.z = Hope[cntModel].rot.z + diffKeyDest[cntModel].rot.z * ((float)m_data.blendCounter / (float)m_data.blendFrame);

			// ���f���̈ʒu�A������ݒ肷��
			CModelManager::GetInstance().SetPos(m_modelIndex[cntModel], D3DXVECTOR3(HopeDest[cntModel].pos.x, HopeDest[cntModel].pos.y, HopeDest[cntModel].pos.z));
			CModelManager::GetInstance().SetRot(m_modelIndex[cntModel], D3DXVECTOR3(HopeDest[cntModel].rot.x, HopeDest[cntModel].rot.y, HopeDest[cntModel].rot.z));
		}
		else if (m_data.MotionEnd == false)
		{
			// ���f���̈ʒu�A������ݒ肷��
			CModelManager::GetInstance().SetPos(m_modelIndex[cntModel], D3DXVECTOR3(Hope[cntModel].pos.x, Hope[cntModel].pos.y, Hope[cntModel].pos.z));
			CModelManager::GetInstance().SetRot(m_modelIndex[cntModel], D3DXVECTOR3(Hope[cntModel].rot.x, Hope[cntModel].rot.y, Hope[cntModel].rot.z));
		}
	}

	if (m_data.bBlend == true && m_data.blendCounter >= m_data.blendFrame)
	{// �u�����h�t���[���S�������I�������
		m_data.blendCounter = 0;                      // �u�����h�J�E���^�[�[��

		// �u�����h������̃��[�V�����ɂ���
		m_pMotion = m_pBlendMotion;
		m_data.bLoop = m_pMotion->bLoop;	                          // ���[�v
		m_data.key = m_data.keyBlend;									  // �������Ă���L�[
		m_data.nextKey = m_data.nextKeyBlend;                             // ���̃L�[
		m_data.counterMotion = m_data.counterMotionBlend;				  // �t���[���J�E���^�[
		m_data.counterKey = m_data.counterKeyBlend;						  // �L�[�J�E���^�[
		m_data.MotionEnd = m_data.MotionEndBlend;                           // ���[�V����

		m_data.bBlend = false;                         // �u�����h�I��
	}

	return m_data.MotionEnd;
}

//----------------------------------------------------
//
// ���f���Ǘ��N���X
//
//----------------------------------------------------

// �ÓI�����o�ϐ�
CMotionManager CMotionManager::m_instance{};     // �����̃C���X�^���X

//----------------------------------------------------
// ���f�����g��
//----------------------------------------------------
Index CMotionManager::Create(void)
{
	// ���f���̐���
	CMotion* pMotion = nullptr;
	pMotion = new CMotion;
	if (pMotion == nullptr)
	{// �������s
		return INVALID_ID; // �G���[�l
	}

	m_apMotion.push_back(pMotion); // ���f����ǉ�

	return m_apMotion.size() - 1;
}

//----------------------------------------------------
// �j������
//----------------------------------------------------
void CMotionManager::Release(void)
{
	SAFE_DELETE_ARRAY(m_apMotion); // �S�Ẵ��f�����A���C�j�b�g
}