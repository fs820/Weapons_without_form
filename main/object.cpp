//------------------------------------------
//
//描画用の処理[object.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "object.h"
#include "manager.h"

using namespace DirectX; // DirectX名前空間の使用

//---------------------------------------
//
// オブジェクト基本クラス
//
//---------------------------------------
array<vector<CObject*>, 8> CObject::m_apObject{};   // オブジェクトのポインタ配列
int CObject::m_nAll = 0;

//------------------------------
// 全更新
//------------------------------
void CObject::UpdateAll(const context::UpdateContext updateContext)
{
	CallCollision(); // 衝突判定を呼び出す

	for (auto& priority : m_apObject)
	{// priorityを回す
		for (Index cntObject = 0; cntObject < priority.size();)
		{// priority内を回す
			if (priority[cntObject] != nullptr)
			{// オブジェクトがある
				priority[cntObject]->Update(); // オブジェクトの更新

				if (priority[cntObject]->IsRelease())
				{// 削除
					Release(priority[cntObject]); // オブジェクトを破棄
					continue;                     // 削除時はカウント動かさない
				}
			}
			else
			{// オブジェクトがない
				do
				{// nullでない要素が来るまで
					SwapRemove(priority, cntObject); // オブジェクトポインタを配列から除去
					if (priority[cntObject] != nullptr)
					{// 存在する
						priority[cntObject]->m_ID = cntObject; // インデックス更新
						break;
					}
				} while (true);
			}
			++cntObject; // カウント進める
		}
	}
}

//------------------------------
// 全描画
//------------------------------
void CObject::DrawAll(const context::DrawContext drawContext, bool bAfterimage)
{
	for (auto& priority : m_apObject)
	{// priority配列
		for (auto& pObject : priority)
		{// オブジェクト配列
			if (pObject != nullptr && (!bAfterimage || pObject->IsAfterimage()))
			{// 存在する
				pObject->Draw();
			}
		}
	}
}

//------------------------------
// プライオリティ描画
//------------------------------
void CObject::DrawPriority(Index priority, const context::DrawContext drawContext, bool bAfterimage)
{
	if (priority < m_apObject.size())
	{
		for (auto& pObject : m_apObject[priority])
		{// オブジェクト配列
			if (pObject != nullptr && (!bAfterimage || pObject->IsAfterimage()))
			{// 存在する
				pObject->Draw();
			}
		}
	}
}

//------------------------------
// 全破棄
//------------------------------
void CObject::ReleaseAll(void)
{
	for (auto& priority : m_apObject)
	{// priority配列
		for (auto& pObject : priority)
		{// オブジェクト配列
			SAFE_UNINIT(pObject); // オブジェクトのアンイニットを呼び出す
		}
	}
	m_apObject.fill(vector<CObject*>()); // 全てのオブジェクトポインタをクリア
	m_nAll = 0;                          // 全オブジェクト数をリセット
}

//------------------------------
// 初期化
//------------------------------
void CObject::Init(TYPE type)
{
	// オブジェクト配列に自分を登録する
	m_apObject[m_Priority].push_back(this);
	m_ID = m_apObject[m_Priority].size() - 1;
	m_nAll++;

	m_transform = {}; // トランスフォーム初期化
	m_type = type;    // オブジェクトの種類を設定
}

//------------------------------
// 破棄
//------------------------------
void CObject::Release(CObject* pObject)
{
	if (pObject == nullptr) return;                                 // nullチェック
	Index priority = pObject->GetPriority(), id = pObject->GetID(); // 配列から削除するための情報を削除前にもらう
	SAFE_UNINIT(pObject);                                           // オブジェクトを削除
	do
	{// nullでない要素が来るまで
		SwapRemove(m_apObject[priority], id); // オブジェクトポインタを配列から除去
		if (m_apObject[priority][id] != nullptr)
		{// 存在する
			m_apObject[priority][id]->m_ID = id; // インデックス更新
			break;
		}
	} while (true);
	--m_nAll; // オブジェクト数を減らす
}

//------------------------------
// OnCollosion呼び出し
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

			// ゲストはホストの直後から全体を回す
			for (Index guestPri = hostPri; guestPri < m_apObject.size(); ++guestPri)
			{
				auto& guestVec = m_apObject[guestPri];

				// guestPri == hostPri のときはguestIdxをhostIdx + 1から開始
				Index startGuestIdx = (guestPri == hostPri) ? hostIdx + 1 : 0;

				for (Index guestIdx = startGuestIdx; guestIdx < guestVec.size(); ++guestIdx)
				{
					CObject* pGuest = guestVec[guestIdx];
					if (pGuest == nullptr || !pGuest->m_bCollision || pGuest->m_type == TYPE::None) continue;

					if (CallCollisionHelper(*pHost, *pGuest))
					{
						pHost->OnCollision(*pGuest);

						// pHostがOnCollisionで破棄されていないか確認（nullptrチェック）
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
// 衝突判定ヘルパー関数
//------------------------------
bool CObject::CallCollisionHelper(const CObject& HostObject, const CObject& GuestObject)
{
	//------------------------------------------------------
	// ひとまず円の当たり判定で行う (必要に応じて矩形判定)
	//------------------------------------------------------

	// トランスフォームをサイズを取得
	Transform HostTrans = HostObject.GetTransform();
	D3DXVECTOR3 HostSize = HostObject.GetSize();
	HostSize.x *= HostTrans.scale.x;
	HostSize.y *= HostTrans.scale.y;
	HostSize.z *= HostTrans.scale.z;
	Transform GuestTrans = GuestObject.GetTransform();
	D3DXVECTOR3 GuestSize = GuestObject.GetSize();
	GuestSize.x *= GuestTrans.scale.x;
	GuestSize.z *= GuestTrans.scale.z;

	// 直径
	float HostLength = D3DXVec3Length(&HostSize);
	float GuestLength = D3DXVec3Length(&GuestSize);

	//// スクリーン座標に変換
	//D3DXVECTOR2 screensize = {};
	//CManager::GetRenderer().GetDxScreenSize(&screensize);
	//HostTrans.pos.x *= screensize.x;
	//HostTrans.pos.y *= screensize.y;
	//GuestTrans.pos.x *= screensize.x;
	//GuestTrans.pos.y *= screensize.y;

	D3DXVECTOR3 Space = GuestTrans.pos - HostTrans.pos; // ホストとゲストの位置ベクトルを計算
	if (D3DXVec3Length(&Space) < (HostLength + GuestLength) * 0.5f)
	{// 円が接している
		return true;
	}
	return false;
}