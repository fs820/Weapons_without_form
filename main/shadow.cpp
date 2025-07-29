//------------------------------------------
//
// 影[shadow.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "shadow.h"
#include "renderer.h"

using namespace DirectX3D; // DirectX名前空間の使用

//-----------------------------
// 
// 影クラス
// 
//-----------------------------

LPDIRECT3DVERTEXBUFFER9 CShadow::m_pVtxBuff{}; // 静的メンバ変数の定義

//------------------------------
// 生成処理
//------------------------------
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, Index priority)
{
	CShadow* pShadow = new CShadow(priority); // インスタンス生成

	if (pShadow == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pShadow->Init(pos, rot, scale)))
	{
		SAFE_DELETE(pShadow);
		return nullptr;
	}

	return pShadow;
}

//------------------------------
// 生成処理
//------------------------------
CShadow* CShadow::Create(DirectX::Transform transform, Index priority)
{
	CShadow* pShadow = new CShadow(priority); // インスタンス生成

	if (pShadow == nullptr)
	{// 生成失敗
		return nullptr;
	}

	// 初期化
	if (FAILED(pShadow->Init(transform)))
	{
		SAFE_DELETE(pShadow);
		return nullptr;
	}

	return pShadow;
}

//------------------------------
// 初期化処理
//------------------------------
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	if (m_pVtxBuff == nullptr)
	{
		CManager::GetRenderer().CreateViewportVertexBuffer(&m_pVtxBuff, Black(0.1f)); // ビューポートの頂点バッファを生成
	}
	CObjectX::Init(pos, rot, scale,MODELTAG::Shadow, TYPE::Shadow);
	return S_OK;
}

//------------------------------
// 初期化処理
//------------------------------
HRESULT CShadow::Init(DirectX::Transform transform)
{
	if (m_pVtxBuff == nullptr)
	{
		CManager::GetRenderer().CreateViewportVertexBuffer(&m_pVtxBuff, Black(0.1f)); // ビューポートの頂点バッファを生成
	}
	CObjectX::Init(transform, MODELTAG::Shadow, TYPE::Shadow);
	return S_OK;
}

//------------------------------
// 描画処理
//------------------------------
void CShadow::Draw(void)
{
	if (m_pVtxBuff == nullptr)
	{
		return; // 頂点バッファがない場合は描画しない
	}

	// レンダラーからデバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	D3DCAPS9 caps{};
	if (SUCCEEDED(pDevice->GetDeviceCaps(&caps)))
	{
		if (!(caps.StencilCaps & (D3DSTENCILCAPS_INCR | D3DSTENCILCAPS_DECR)))
		{
			return; // ステンシル操作がサポートされていない場合は描画しない
		}

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); // ライティングを無効化

		// ステンシル用の設定
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);          // ステンシルバッファを有効化
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);          // Zバッファ書き込みを無効化
		//pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000); // カラーバッファ書き込みを無効化

		if (caps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
		{
			// 両面ステンシルモードを有効化
			pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);     // ステンシル関数を常に通過
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // ステンシルパスをキープに設定
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR); // ステンシルZフェイルをインクリメントに設定
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // ステンシルフェイルをキープに設定

			pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);  // ステンシルパスをキープに設定
			pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR); // ステンシルZフェイルをデクリメントに設定
			pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);  // ステンシルフェイルをキープに設定

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // カリングモードを無効化

			CObjectX::Draw(); // 描画処理を呼び出す

			// 両面ステンシルモードを無効化
			pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // カリングモードをCCWに設定
		}
		else
		{
			pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);     // ステンシル関数を常に通過
			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // ステンシルパスをキープに設定
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR); // ステンシルZフェイルをインクリメントに設定
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // ステンシルフェイルをキープに設定

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); // カリングモードをCWに設定

			CObjectX::Draw(); // 描画処理を呼び出す

			pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);  // ステンシルパスをキープに設定
			pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR); // ステンシルZフェイルをデクリメントに設定
			pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);  // ステンシルフェイルをキープに設定

			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // カリングモードをCCWに設定

			CObjectX::Draw(); // 描画処理を呼び出す
		}

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F); // カラーバッファ書き込みを有効化

		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);                   // ステンシルリファレンス値を設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);          // ステンシル関数を等しいに設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);     // ステンシルパスをキープに設定
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);    // ステンシルZフェイルをキープに設定
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);     // ステンシルフェイルをキープに設定

		// 頂点バッファ
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(DirectX2D::Vertex));

		// 頂点フォーマットの設定
		pDevice->SetFVF(DirectX2D::FVF_VERTEX);

		// ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			0,                  // インデックス
			2                   //ポリゴン数
		);

		// 元に戻す
		CManager::GetRenderer().SetRender();
	}
	else
	{
		return; // デバイスの取得に失敗した場合は描画しない
	}
}