//------------------------------------------
//
// �K�w�\�������̒�` [hierarchy.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

namespace hierarchy
{
	struct PartsInfo
	{
		Index modelTag;     // ���f���̃^�O
		int parentIdx;      // �e�̃C���f�b�N�X
		D3DXVECTOR3 offSet; // �I�t�Z�b�g
	};
}

//----------------------------------------------
// �K�w�\���Ǘ��N���X �V���O���g�� �R�s�[�֎~
//----------------------------------------------
class CHierarchyManager final
{
// ���J
public:
	static CHierarchyManager& GetInstance(void)
	{
		static CHierarchyManager instance; // �ÓI�C���X�^���X
		return instance;
	}

	CHierarchyManager(const CHierarchyManager&) = delete;
	CHierarchyManager& operator=(const CHierarchyManager&) = delete;

	void Register(vector<hierarchy::PartsInfo> hierarchy) { m_hierarchyList.push_back(hierarchy); }
	void Unregister(void) { m_hierarchyList.clear(); }

	const vector<hierarchy::PartsInfo>& GetHierarchy(Index idx) const
	{
		static const vector<hierarchy::PartsInfo> null{};
		return (idx >= 0 && idx < m_hierarchyList.size()) ? m_hierarchyList[idx] : null;
	}

// ����J
private:
	CHierarchyManager() : m_hierarchyList{} {}
	~CHierarchyManager() = default;

	vector<vector<hierarchy::PartsInfo>> m_hierarchyList; // �K�w�\�����̃��X�g
};