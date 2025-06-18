//------------------------------------------
//
// モデル処理の定義 [model.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

//-----------------------------------------
// モデルクラス
//-----------------------------------------
class CModel final
{
	// 公開
public:
	// サブセット情報
	using SubSetInfo = struct
	{
		bool bUse;      // 使われているか
		DWORD AttribId; // ID
		size_t texIdx;  // テクスチャインデックス
	};
	// 情報
	using Info = struct
	{
		LPD3DXMESH pMesh;		  // メッシュ(頂点情報へのポインタ)
		LPD3DXBUFFER pBuffMat;	  // マテリアルへのポインタ
		D3DXMATERIAL* pMat;	      // マテリアルデータへのポインタ
		DWORD dwNumMat;			  // マテリアルの数
		DWORD dwNumSubSet;        // サブセット数
		SubSetInfo* pSubSet;      // サブセット情報のポインタ
		int nIdxParent;	          // 親モデルのインデックス
		const D3DXVECTOR3 offSet; // オフセット
		D3DXVECTOR3 pos;          // 位置
		D3DXVECTOR3 rot;          // 方向
		D3DXVECTOR3 scale;        // スケール
		D3DXMATRIX mtxWorld;      // ワールドマトリックス
	};

	CModel(const string_view sPass, const D3DXVECTOR3 offSet, const int nIdxParent = -1) :m_sPass(sPass), m_info{ .nIdxParent = nIdxParent,.offSet = offSet }, m_countUse{}, m_pModel{} {}
	~CModel() = default;

	HRESULT Init(void);
	void Uninit(void);
	void Draw(void);
	string_view GetPass(void) const { return m_sPass; }
	const Info GetInfo(void) const { return m_info; }

	void SetPos(const D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetScale(const D3DXVECTOR3 scale) { m_info.scale = scale; }

	void SetParent(const int nIdxParent);

	void AddUseCount(void) { m_countUse++; }

	// 非公開
private:
	const string_view m_sPass; // パス
	const CModel* m_pModel;    // 親モデルのポインタ

	Info m_info;
	size_t m_countUse;             // どれだけ使ってるか
};

//----------------------------------------------
// モデル管理クラス シングルトン コピー禁止
//----------------------------------------------
class CModelManager final
{
	// 公開
public:
	// 最初から予め読み込むモデル列挙
	using TYPE = enum
	{
		PLAYER = 0,
		TYPE_MAX
	};

	static CModelManager& GetInstance(void) { return m_instance; };

	CModelManager(const CModelManager&) = delete;
	CModelManager& operator=(const CModelManager&) = delete;

	HRESULT Load(void);
	void Unload(void);

	Index Register(const string_view sModelPass, const D3DXVECTOR3 offSet, const int nIdxParent = -1);
	void Draw(size_t idx) const { if (idx >= 0 && idx < m_nNumAll) m_apModel[idx]->Draw(); }

	const CModel* const GetModel(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx] : nullptr; }
	string_view GetPass(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetPass() : string_view(); }
	const LPD3DXMESH GetMesh(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pMesh : nullptr; }
	const LPD3DXBUFFER GetBuffMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pBuffMat : nullptr; }
	const D3DXMATERIAL* GetMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pMat : nullptr; }
	DWORD GetNumMat(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().dwNumMat : 0; }
	DWORD GetNumSubSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().dwNumSubSet : 0; }
	const CModel::SubSetInfo* GetSubSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pSubSet : nullptr; }
	const int GetParent(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().nIdxParent : 0; }
	const D3DXVECTOR3 GetOffSet(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().offSet : 0; }
	D3DXMATRIX GetMatrix(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().mtxWorld : 0; }

	D3DXVECTOR3 GetPos(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().pos : 0; }
	D3DXVECTOR3 GetRot(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().rot : 0; }
	D3DXVECTOR3 GetScale(size_t idx) const { return (idx >= 0 && idx < m_nNumAll) ? m_apModel[idx]->GetInfo().scale : 0; }
	void SetPos(size_t idx, const D3DXVECTOR3 pos) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetPos(pos); }
	void SetRot(size_t idx, const D3DXVECTOR3 rot) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetRot(rot); }
	void SetScale(size_t idx, const D3DXVECTOR3 scale) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetScale(scale); }

	void SetParent(size_t idx, const int nIdxParent) { if (idx >= 0 && idx < m_nNumAll)  m_apModel[idx]->SetParent(nIdxParent); }

	// 非公開
private:
	static constexpr size_t MAX_MODEL = 128; // モデル最大数

	static CModelManager m_instance;  // 自分のインスタンス

	CModelManager() : m_apModel{}, m_nNumAll{},
		// 規定モデルリスト
		m_typePass
	{
		"data\\MODEL\\player000.x" // プレイヤー
	},  // 規定モデル親
		m_typeParent
	{
		-1                         // プレイヤー
	},  // 規定モデルオフセット
		m_typeOffSet
	{
		{0.0f,0.0f,0.0f}           // プレイヤー
	} {
	}
	~CModelManager() = default;

	const string_view m_typePass[TYPE_MAX];   // 規定モデルリスト
	const int m_typeParent[TYPE_MAX];         // 規定モデル親
	const D3DXVECTOR3 m_typeOffSet[TYPE_MAX]; // 規定モデルオフセット
	CModel* m_apModel[MAX_MODEL];             // モデル情報
	size_t m_nNumAll;                         // モデル数
};