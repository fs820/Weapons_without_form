//----------------------------------------------------
//
// モーション処理 [motion.cpp]
// Author: fuma sato
//
//----------------------------------------------------
#include "motion.h"
#include "manager.h"
#include "model.h"

using namespace motion; // モーション空間の使用

//----------------------------------------------------
//
// モデルクラス
//
//----------------------------------------------------

//----------------------------------------------------
// モーションセット
//----------------------------------------------------
void CMotion::Set(const Motion* pMotion, bool bBlend, int blendFrame)
{
	if (bBlend && m_pMotion != nullptr && m_pMotion->aKey.size() > 1)
	{// ブレンドする
		m_pBlendMotion = pMotion;
		m_data.bLoopBlend = pMotion->bLoop;	// ループの代入
		m_data.keyBlend = 0;				// 今数えているキーを初期化
		if (pMotion->aKey.size() > 1)
		{// 一キー以上あるなら
			m_data.nextKeyBlend = m_data.keyBlend + 1; // 次のキーを設定
		}
		else
		{// ないならば
			m_data.nextKeyBlend = m_data.keyBlend;	// 次のキーを設定
		}
		m_data.counterMotionBlend = 0;	// フレームカウンターをゼロ
		m_data.counterKeyBlend = 0;	    // キーカウンターをゼロ
		m_data.MotionEndBlend = false;  // モーションは終わっていない

		m_data.blendFrame = blendFrame; // ブレンドフレーム
		m_data.blendCounter = 0;        // ブレンドカウンター
		m_data.bBlend = true;           // ブレンドする
	}
	else
	{// ブレンドしない
		m_pMotion = pMotion;
		m_data.bLoop = pMotion->bLoop;	// ループの代入
		m_data.key = 0;				    // 今数えているキーを初期化
		if (pMotion->aKey.size() > 1)
		{// 一キー以上あるなら
			m_data.nextKey = m_data.key + 1;  // 次のキーを設定
		}
		else
		{// ないならば
			m_data.nextKey = m_data.key;	// 次のキーを設定
		}
		m_data.counterMotion = 0;	// フレームカウンターをゼロ
		m_data.counterKey = 0;		// キーカウンターをゼロ
		m_data.MotionEnd = false;   // モーションは終わっていない
		m_data.bBlend = false;      // ブレンドしない

		//ブレンド用の変数も初期化しておく
		m_pBlendMotion = nullptr;
		m_data.bLoopBlend = false;	            // ループの代入
		m_data.keyBlend = INVALID_ID;			// 今数えているキーを初期化
		m_data.nextKeyBlend = INVALID_ID;       // 次のキーを設定
		m_data.counterMotionBlend = INVALID_ID; // フレームカウンターをゼロ
		m_data.counterKeyBlend = INVALID_ID;	// キーカウンターをゼロ
		m_data.MotionEndBlend = false;          // モーションは終わっていない

		m_data.blendFrame = 0;    // ブレンドフレーム
		m_data.blendCounter = 0;  // ブレンドカウンター
	}
}

//----------------------------------------------------
// モーションの更新
//----------------------------------------------------
bool CMotion::Update(void)
{
	Index modelNum = m_modelIndex.size(); // 登録されているモデル数
	if (modelNum > m_pMotion->aKey[0].aParts.size())
	{
		return false;
	}

	//今のモーション
	vector<Parts> nextKey(modelNum); // 次のキー
	vector<Parts> nowKey(modelNum);  // 今のキー
	vector<Parts> diffKey(modelNum); // キーの差分
	vector<Parts> Hope(modelNum);	 // 希望の値

	//ブレンド先のモーション
	vector<Parts> nextKeyBlend(modelNum); // 次のキー
	vector<Parts> nowKeyBlend(modelNum);  // 今のキー
	vector<Parts> diffKeyBlend(modelNum); // キーの差分
	vector<Parts> HopeBlend(modelNum);	  // 希望の値

	//結果
	vector<Parts> diffKeyDest(modelNum); // キーの差分
	vector<Parts> HopeDest(modelNum);	 // 希望の値

	if (m_data.nextKey < m_pMotion->aKey.size())
	{// モーションの全体キーより値が低い場合

		++m_data.counterMotion;	// モーションカウント
		++m_data.counterKey;		// キーカウント

		// 全モデルパーツの更新
		for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
		{
			nextKey[cntModel] = m_pMotion->aKey[m_data.nextKey].aParts[cntModel]; // 次のキー
			nowKey[cntModel] = m_pMotion->aKey[m_data.key].aParts[cntModel];      // 今のキー

			// キーの差分(位置)を求める
			diffKey[cntModel].pos.x = (nextKey[cntModel].pos.x - nowKey[cntModel].pos.x);
			diffKey[cntModel].pos.y = (nextKey[cntModel].pos.y - nowKey[cntModel].pos.y);
			diffKey[cntModel].pos.z = (nextKey[cntModel].pos.z - nowKey[cntModel].pos.z);

			// キーの差分(向き)を求める
			diffKey[cntModel].rot.x = (nextKey[cntModel].rot.x - nowKey[cntModel].rot.x);
			diffKey[cntModel].rot.y = (nextKey[cntModel].rot.y - nowKey[cntModel].rot.y);
			diffKey[cntModel].rot.z = (nextKey[cntModel].rot.z - nowKey[cntModel].rot.z);

			// オフセットに足す値を求める
			Hope[cntModel].pos.x = nowKey[cntModel].pos.x + diffKey[cntModel].pos.x * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].pos.y = nowKey[cntModel].pos.y + diffKey[cntModel].pos.y * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].pos.z = nowKey[cntModel].pos.z + diffKey[cntModel].pos.z * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);

			// 向きを求める
			Hope[cntModel].rot.x = nowKey[cntModel].rot.x + diffKey[cntModel].rot.x * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].rot.y = nowKey[cntModel].rot.y + diffKey[cntModel].rot.y * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
			Hope[cntModel].rot.z = nowKey[cntModel].rot.z + diffKey[cntModel].rot.z * ((float)m_data.counterKey / (float)m_pMotion->aKey[m_data.key].frame);
		}

		if (m_data.counterKey >= m_pMotion->aKey[m_data.key].frame)
		{// フレーム全部数え終わったら
			m_data.counterKey = 0;          	        // キーカウントゼロ
			++m_data.nextKey;	                        // 次のキーへカウント
			++m_data.key;		                        // 次のキーへカウント
			m_data.nextKey %= m_pMotion->aKey.size();	// キーの総数分に制限する
			m_data.key %= m_pMotion->aKey.size();		// キーの総数分に制限する
			if (m_data.nextKey == 0 && m_data.bLoop == false)
			{// ループしないモーションは終わらせる
				m_data.nextKey = m_pMotion->aKey.size();	// 計算の条件式に入らないようにする
				m_data.counterMotion = 0;			        // モーションカウントゼロ
				m_data.MotionEnd = true;		            // モーション終わり
			}
		}
	}

	if (m_data.bBlend == true && m_data.nextKeyBlend < m_pBlendMotion->aKey.size())
	{// ブレンドするかつモーションの全体キーより値が低い場合
		++m_data.counterMotionBlend;	// モーションカウント
		++m_data.counterKeyBlend;		// キーカウント

		// 全モデルパーツの更新
		for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
		{
			nextKeyBlend[cntModel] = m_pBlendMotion->aKey[m_data.nextKeyBlend].aParts[cntModel]; // 次のキー
			nowKeyBlend[cntModel] = m_pBlendMotion->aKey[m_data.keyBlend].aParts[cntModel];      // 今のキー

			// キーの差分(位置)を求める
			diffKeyBlend[cntModel].pos.x = (nextKeyBlend[cntModel].pos.x - nowKeyBlend[cntModel].pos.x);
			diffKeyBlend[cntModel].pos.y = (nextKeyBlend[cntModel].pos.y - nowKeyBlend[cntModel].pos.y);
			diffKeyBlend[cntModel].pos.z = (nextKeyBlend[cntModel].pos.z - nowKeyBlend[cntModel].pos.z);

			// キーの差分(向き)を求める
			diffKeyBlend[cntModel].rot.x = (nextKeyBlend[cntModel].rot.x - nowKeyBlend[cntModel].rot.x);
			diffKeyBlend[cntModel].rot.y = (nextKeyBlend[cntModel].rot.y - nowKeyBlend[cntModel].rot.y);
			diffKeyBlend[cntModel].rot.z = (nextKeyBlend[cntModel].rot.z - nowKeyBlend[cntModel].rot.z);

			// オフセットに足す値を求める
			HopeBlend[cntModel].pos.x = nowKeyBlend[cntModel].pos.x + diffKeyBlend[cntModel].pos.x * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].pos.y = nowKeyBlend[cntModel].pos.y + diffKeyBlend[cntModel].pos.y * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].pos.z = nowKeyBlend[cntModel].pos.z + diffKeyBlend[cntModel].pos.z * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);

			// 向きを求める
			HopeBlend[cntModel].rot.x = nowKeyBlend[cntModel].rot.x + diffKeyBlend[cntModel].rot.x * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].rot.y = nowKeyBlend[cntModel].rot.y + diffKeyBlend[cntModel].rot.y * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
			HopeBlend[cntModel].rot.z = nowKeyBlend[cntModel].rot.z + diffKeyBlend[cntModel].rot.z * ((float)m_data.counterKeyBlend / (float)m_pBlendMotion->aKey[m_data.keyBlend].frame);
		}

		if (m_data.counterKeyBlend >= m_pBlendMotion->aKey[m_data.keyBlend].frame)
		{// フレーム全部数え終わったら
			m_data.counterKeyBlend = 0;          	        // キーカウントゼロ
			++m_data.nextKeyBlend;	                    // 次のキーへカウント
			++m_data.keyBlend;		                    // 次のキーへカウント
			m_data.nextKeyBlend %= m_pBlendMotion->aKey.size();	// キーの総数分に制限する
			m_data.keyBlend %= m_pBlendMotion->aKey.size();		// キーの総数分に制限する

			if (m_data.nextKeyBlend == 0 && m_data.bLoopBlend == false)
			{// ループしないモーションは終わらせる
				m_data.nextKeyBlend = m_pBlendMotion->aKey.size();	// 計算の条件式に入らないようにする
				m_data.counterMotionBlend = 0;			    // モーションカウントゼロ
				m_data.MotionEndBlend = true;		            // モーション終わり
			}
		}
	}

	// 全モデルパーツの更新
	for (Index cntModel = 0; cntModel < modelNum; ++cntModel)
	{
		if (m_data.bBlend == true)
		{
			++m_data.blendCounter; //ブレンドカウンター

			// キーの差分(位置)を求める
			diffKeyDest[cntModel].pos.x = (HopeBlend[cntModel].pos.x - Hope[cntModel].pos.x);
			diffKeyDest[cntModel].pos.y = (HopeBlend[cntModel].pos.y - Hope[cntModel].pos.y);
			diffKeyDest[cntModel].pos.z = (HopeBlend[cntModel].pos.z - Hope[cntModel].pos.z);

			// キーの差分(向き)を求める
			diffKeyDest[cntModel].rot.x = (HopeBlend[cntModel].rot.x - Hope[cntModel].rot.x);
			diffKeyDest[cntModel].rot.y = (HopeBlend[cntModel].rot.y - Hope[cntModel].rot.y);
			diffKeyDest[cntModel].rot.z = (HopeBlend[cntModel].rot.z - Hope[cntModel].rot.z);

			// オフセットに足す値を求める
			HopeDest[cntModel].pos.x = Hope[cntModel].pos.x + diffKeyDest[cntModel].pos.x * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].pos.y = Hope[cntModel].pos.y + diffKeyDest[cntModel].pos.y * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].pos.z = Hope[cntModel].pos.z + diffKeyDest[cntModel].pos.z * ((float)m_data.blendCounter / (float)m_data.blendFrame);

			// 向きを求める
			HopeDest[cntModel].rot.x = Hope[cntModel].rot.x + diffKeyDest[cntModel].rot.x * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].rot.y = Hope[cntModel].rot.y + diffKeyDest[cntModel].rot.y * ((float)m_data.blendCounter / (float)m_data.blendFrame);
			HopeDest[cntModel].rot.z = Hope[cntModel].rot.z + diffKeyDest[cntModel].rot.z * ((float)m_data.blendCounter / (float)m_data.blendFrame);

			// モデルの位置、向きを設定する
			CModelManager::GetInstance().SetPos(m_modelIndex[cntModel], D3DXVECTOR3(HopeDest[cntModel].pos.x, HopeDest[cntModel].pos.y, HopeDest[cntModel].pos.z));
			CModelManager::GetInstance().SetRot(m_modelIndex[cntModel], D3DXVECTOR3(HopeDest[cntModel].rot.x, HopeDest[cntModel].rot.y, HopeDest[cntModel].rot.z));
		}
		else if (m_data.MotionEnd == false)
		{
			// モデルの位置、向きを設定する
			CModelManager::GetInstance().SetPos(m_modelIndex[cntModel], D3DXVECTOR3(Hope[cntModel].pos.x, Hope[cntModel].pos.y, Hope[cntModel].pos.z));
			CModelManager::GetInstance().SetRot(m_modelIndex[cntModel], D3DXVECTOR3(Hope[cntModel].rot.x, Hope[cntModel].rot.y, Hope[cntModel].rot.z));
		}
	}

	if (m_data.bBlend == true && m_data.blendCounter >= m_data.blendFrame)
	{// ブレンドフレーム全部数え終わったら
		m_data.blendCounter = 0;                      // ブレンドカウンターゼロ

		// ブレンド先を今のモーションにする
		m_pMotion = m_pBlendMotion;
		m_data.bLoop = m_pMotion->bLoop;	                          // ループ
		m_data.key = m_data.keyBlend;									  // 今数えているキー
		m_data.nextKey = m_data.nextKeyBlend;                             // 次のキー
		m_data.counterMotion = m_data.counterMotionBlend;				  // フレームカウンター
		m_data.counterKey = m_data.counterKeyBlend;						  // キーカウンター
		m_data.MotionEnd = m_data.MotionEndBlend;                           // モーション

		m_data.bBlend = false;                         // ブレンド終了
	}

	return m_data.MotionEnd;
}

//----------------------------------------------------
//
// モデル管理クラス
//
//----------------------------------------------------

// 静的メンバ変数
CMotionManager CMotionManager::m_instance{};     // 自分のインスタンス

//----------------------------------------------------
// モデルを使う
//----------------------------------------------------
Index CMotionManager::Create(void)
{
	// モデルの生成
	CMotion* pMotion = nullptr;
	pMotion = new CMotion;
	if (pMotion == nullptr)
	{// 生成失敗
		return INVALID_ID; // エラー値
	}

	m_apMotion.push_back(pMotion); // モデルを追加

	return m_apMotion.size() - 1;
}

//----------------------------------------------------
// 破棄処理
//----------------------------------------------------
void CMotionManager::Release(void)
{
	SAFE_DELETE_ARRAY(m_apMotion); // 全てのモデルをアンイニット
}