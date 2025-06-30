//------------------------------------------
//
// モーション処理の定義 [motion.h]
// Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

namespace motion
{
	// 1つのパーツ構成
	struct Parts
	{
		D3DXVECTOR3 pos; // 位置
		D3DXVECTOR3 rot; // 向き
	};

	// 1つのキー構成
	struct Key
	{
		Index frame;			  // 再生フレーム
		vector<Parts> aParts; // 各パーツのキー要素
	};

	// 1つのモーション構成
	struct Motion
	{
		bool bLoop;		  // ループするかどうか
		vector<Key> aKey; // キー情報
	};

	// モーション動作用データ
	struct Data
	{
		bool bLoop;                     // ループするかどうか
		Index key;						// 現在のキーNo.
		Index nextKey;	                // 次のキーNo.
		Index counterKey;				// キーカウンター
		Index counterMotion;			// モーションカウンター
		bool MotionEnd;                 // モーションが終わっているか

		bool bLoopBlend;                // ブレンド先がループするかどうか
		Index keyBlend;					// ブレンド先の現在のキーNo.
		Index nextKeyBlend;	            // ブレンド先の次のキーNo.
		Index counterKeyBlend;		    // ブレンドキーカウンター
		Index counterMotionBlend;		// ブレンドモーションカウンター
		bool MotionEndBlend;            // ブレンド先のモーションが終わっているか

		bool bBlend;                    // ブレンドするか
		Index blendFrame;               // ブレンドフレーム
		Index blendCounter;             // ブレンドカウンター
	};

	// タグごとのモーションの塊
	struct Source
	{
		vector<Motion> aMotion; // モーション構成リスト
	};
}

//-----------------------------------------
// モーションクラス
//-----------------------------------------
class CMotion final
{
	// クラスusing
	using Motion = motion::Motion;
	using Data = motion::Data;
	using Key = motion::Key;
	using Parts = motion::Parts;
// 公開
public:
	CMotion() : m_modelIndex{}, m_pMotion{}, m_pBlendMotion{}, m_data{} {}
	~CMotion() = default;

	void RegisterModel(const vector<Index> modelIndex) { m_modelIndex = modelIndex; };
	void Set(const Motion* pMotion, bool bBlend, int blendFrame);
	bool Update(void);

// 非公開
private:
	vector<Index> m_modelIndex;   // 適応するモデルのインデックス(モデルクラス依存)
	const Motion* m_pMotion;      // 今のモーション情報のポインタ
	const Motion* m_pBlendMotion; // ブレンド先のモーション情報のポインタ
	Data m_data;                  // モーションデータ
};

//----------------------------------------------
// モーション管理クラス シングルトン コピー禁止
//----------------------------------------------
class CMotionManager final
{
	// クラスusing
	using Source = motion::Source;
// 公開
public:
	static CMotionManager& GetInstance(void) { return m_instance; }

	CMotionManager(const CMotionManager&) = delete;
	CMotionManager& operator=(const CMotionManager&) = delete;

	void Load(const vector<Source> aSource) { m_aSource = aSource; }

	HRESULT Register(const Source source) { m_aSource.push_back(source); }

	Index Create(void);
	void Release(void);

	void RegisterModel(Index idx, const vector<Index> modelIndex) { if (idx >= 0 && idx < m_apMotion.size()) m_apMotion[idx]->RegisterModel(modelIndex); }
	void Set(Index idx, Index motionTag, Index motionIdx, bool bBlend = false, int blendFrame = 60) { if (idx >= 0 && idx < m_apMotion.size() && motionTag >= 0 && motionTag < m_aSource.size() && motionIdx >= 0 && motionIdx < m_aSource[motionTag].aMotion.size()) m_apMotion[idx]->Set(&m_aSource[motionTag].aMotion[motionIdx], bBlend, blendFrame); }
	void Update(Index idx) const { if (idx >= 0 && idx < m_apMotion.size()) m_apMotion[idx]->Update(); }
// 非公開
private:
	static CMotionManager m_instance;  // 自分のインスタンス

	CMotionManager() : m_apMotion{}, m_aSource{} {}
	~CMotionManager() = default;

	vector<Source> m_aSource;     // 読み込んだモーションデータの格納庫
	vector<CMotion*> m_apMotion;  // モーションのポインタ配列
};