//------------------------------------------
//
// 階層構造処理の定義 [hierarchy.h]
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
		Index modelTag;     // モデルのタグ
		int parentIdx;      // 親のインデックス
		D3DXVECTOR3 offSet; // オフセット
	};
}

//----------------------------------------------
// 階層構造管理クラス シングルトン コピー禁止
//----------------------------------------------
class CHierarchyManager final
{
// 公開
public:
	static CHierarchyManager& GetInstance(void)
	{
		static CHierarchyManager instance; // 静的インスタンス
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

// 非公開
private:
	CHierarchyManager() : m_hierarchyList{} {}
	~CHierarchyManager() = default;

	vector<vector<hierarchy::PartsInfo>> m_hierarchyList; // 階層構造情報のリスト
};