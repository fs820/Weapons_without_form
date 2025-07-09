//------------------------------------------
//
//描画用の処理[renderer.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "debug.h"
#include "camera.h"
#include "light.h"
#include "gui.h"

using namespace renderer; // 描画スペースの使用
using namespace DirectX;  // DirectXスペースの使用

//---------------------------------------
//
// 描画基本クラス
//
//---------------------------------------

//------------------------------
//初期化処理
//------------------------------
HRESULT CRenderer::Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow)
{
	//オブジェクト生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION); // Direct3Dオブジェクトの生成
	if (m_pD3D==nullptr)
	{
		return E_FAIL;
	}

	//ディスプレイモード
	D3DDISPLAYMODE d3ddm = {};//ダイレクトXディスプレイモードの変数宣言
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// クライアント領域のサイズを取得
	RECT clientRect = {};
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // クライアントサイズの取得

	//バックバッファの設定
	m_d3dpp.hDeviceWindow = hWnd;
	m_d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
	m_d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_d3dpp.Windowed = bWindow;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	m_d3dpp.FullScreen_RefreshRateInHz = m_d3dpp.Windowed ? 0 : d3ddm.RefreshRate;

	DWORD qualityLevels = 0;
	const D3DMULTISAMPLE_TYPE samples[] =
	{
		D3DMULTISAMPLE_16_SAMPLES,
		D3DMULTISAMPLE_8_SAMPLES,
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_2_SAMPLES,
		D3DMULTISAMPLE_NONE
	};

	for (auto sample : samples)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType
		(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			m_d3dpp.BackBufferFormat, m_d3dpp.Windowed,
			sample, &qualityLevels))
		)
		{
			m_d3dpp.MultiSampleType = sample;
			m_d3dpp.MultiSampleQuality = (sample == D3DMULTISAMPLE_NONE) ? 0 : qualityLevels - 1;
			break;
		}
	}

	//デバイス生成
	if (FAILED(m_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice
	)))
	{
		if (FAILED(m_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice
		)))
		{
			if (FAILED(m_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	SetRender(); // 描画設定

	// テクスチャ描画
	if (FAILED(SetTexture())) { return E_FAIL; }

	return S_OK;
}

//------------------------------
//終了処理
//------------------------------
void CRenderer::Uninit(void)
{
	// 頂点バッファ
	SAFE_RELEASE(m_pVtx);

	// Zバッファ
	SAFE_RELEASE(m_pAfterimageZBuff);
	SAFE_RELEASE(m_pBackGroundZBuff);
	SAFE_RELEASE(m_pCharZBuff);
	SAFE_RELEASE(m_pEffectZBuff);
	SAFE_RELEASE(m_pUIZBuff);
	SAFE_RELEASE(m_pZBuff);
	SAFE_RELEASE(m_pBackBufferZBuffer);

	// サーフェイス
	SAFE_RELEASE_ARRAY(m_pAfterimageRender);
	SAFE_RELEASE(m_pBackGroundRender);
	SAFE_RELEASE(m_pCharRender);
	SAFE_RELEASE(m_pEffectRender);
	SAFE_RELEASE(m_pUIRender);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pBackBuffer);

	// テクスチャ
	SAFE_RELEASE_ARRAY(m_pAfterimageTexture);
	SAFE_RELEASE(m_pBackGroundTexture);
	SAFE_RELEASE(m_pCharTexture);
	SAFE_RELEASE(m_pEffectTexture);
	SAFE_RELEASE(m_pUITexture);
	SAFE_RELEASE(m_pTexture);

	SAFE_RELEASE(m_pD3DDevice); // デバイス
	SAFE_RELEASE(m_pD3D);       // オブジェクト
}

//------------------------------
// 更新処理
//------------------------------
void CRenderer::Update(void) const
{
	CManager::GetGui().Update(); // Gui更新

	if (CManager::IsPause())
	{

	}
	else
	{
		CObject::UpdateAll();        // オブジェクトの更新
	}
}

//------------------------------
//描画処理
//------------------------------
HRESULT CRenderer::Draw(void)
{
	//-----------------------------
	// 背景
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::BackGround))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		SetRender(); // 描画設定

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(0); // オブジェクトの描画
		CObject::DrawPriority(1); // オブジェクトの描画
		m_pD3DDevice->EndScene(); // 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
    // 残像
    //-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Afterimage))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		RenderTexture(m_pAfterimageTexture[1], White(0.95f), false, false, true);

		SetRender(); // 描画設定
		CManager::GetCamera()->Set(0);  // カメラ
		CObject::DrawPriority(3, true); // 残像オブジェクトの描画
		m_pD3DDevice->EndScene();       // 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
	// キャラクター
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Char))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		SetRender(); // 描画設定

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(2); // オブジェクトの描画
		RenderTexture(m_pAfterimageTexture[0], WHITE, false, true, true, true); // 残像
		CObject::DrawPriority(3); // オブジェクトの描画
		CObject::DrawPriority(4); // オブジェクトの描画
		m_pD3DDevice->EndScene(); // 描画終了
	}
	else
	{
		return E_FAIL;
	}
	//-----------------------------
	// エフェクト
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Effect))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		SetRender(); // 描画設定

		CManager::GetCamera()->Set(0);
		CObject::DrawPriority(5); // オブジェクトの描画
		CObject::DrawPriority(6); // オブジェクトの描画
		m_pD3DDevice->EndScene(); // 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//-----------------------------
	// UI
	//-----------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::UI))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		SetRender(); // 描画設定
		CObject::DrawPriority(7);  // オブジェクトの描画
		m_pD3DDevice->EndScene();  // 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//----------------------------------------
    // メイン (合わせて1つのテクスチャにする)
    //----------------------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::Main))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		RenderTexture(m_pBackGroundTexture, WHITE, false, false, true, true);   // 背景
		RenderTexture(m_pCharTexture, WHITE, false, true, true, true);          // キャラクター
		RenderTexture(m_pEffectTexture, WHITE, false, true, true, true);        // エフェクト
		RenderTexture(m_pUITexture, WHITE, false, true, true, true);            // UI
		m_pD3DDevice->EndScene();// 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//------------------------------------------
    // バックバッファにできたテクスチャ+αを描画
    //------------------------------------------
	if (FAILED(ChangeTarget(RENDER_TARGET::BackBuffer))) { return E_FAIL; }
	if (FAILED(m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0))) { return E_FAIL; }

	if (SUCCEEDED(m_pD3DDevice->BeginScene()))//描画開始
	{
		RenderTexture(m_pTexture, WHITE, false, false, true, true);

		SetRender();               // 描画設定
		CManager::GetGui().Draw(); // Gui描画
		CDebugProc::Draw();        // デバック表示
		m_pD3DDevice->EndScene();  // 描画終了
	}
	else
	{
		return E_FAIL;
	}

	//バックバッファに表示を切り替える
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);

	// 入れ替え
	swap(m_pAfterimageTexture[0], m_pAfterimageTexture[1]);
	swap(m_pAfterimageRender[0], m_pAfterimageRender[1]);

	return S_OK;
}

//------------------------------
// 頂点設定
//------------------------------
HRESULT CRenderer::SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, span<const DirectX2D::Vertex> vertex, DirectX::Vector3 offSet, float angle)
{
	void* pVtx{};
	if (FAILED((*ppVtxBuffer)->Lock(0, 0, &pVtx, 0))) { return E_FAIL; }
	memcpy(pVtx, vertex.data(), sizeof(DirectX2D::Vertex) * vertex.size());
	if (FAILED((*ppVtxBuffer)->Unlock())) { return E_FAIL; }
	return S_OK;
}

//------------------------------
// 頂点設定
//------------------------------
HRESULT CRenderer::SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, DirectX::Vector2 size, DirectX::Vector3 pos, float angle, DirectX::Color color, DirectX::Vector2 texBlock)
{
	vector<DirectX2D::Vertex> vertex{};
	void* pVtx{};
	if (FAILED((*ppVtxBuffer)->Lock(0, 0, &pVtx, 0))) { return E_FAIL; }
	memcpy(pVtx, vertex.data(), sizeof(DirectX2D::Vertex) * vertex.size());
	if (FAILED((*ppVtxBuffer)->Unlock())) { return E_FAIL; }
	return S_OK;
}

//------------------------------
// 描画設定
//------------------------------
void CRenderer::SetRender(void) const
{
    // レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);			 // カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);     // シェーディングモードをグーローシェードに設定
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                   // Zバッファを有効化
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);              // Zバッファ書き込みを有効化
	m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);         // Zバッファを手前優先にする
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			 // プレイヤーの中に透明度を加える
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // 通常のブレンド
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 通常のブレンド
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);          // アルファテスト
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);       // 超過
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0);                     // 閾値
	m_pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);     // 線のアンチエイリアス (ガイド線の安定)
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);            // ステンシルバッファオフ
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                  // ライト

	SetFog(true, D3DFOG_LINEAR, TRANSLUCENT_WHITE, 1000.0f, 1500.0f); // フォグの設定

	// GPU に合わせた異方性フィルタ設定
	D3DCAPS9 caps;
	m_pD3DDevice->GetDeviceCaps(&caps);
	DWORD maxAniso = min(caps.MaxAnisotropy, static_cast<DWORD>(16));

	// サンブラーステートの設定
	for (size_t cntTex = 0; cntTex < 1; cntTex++)
	{
		// フィルター設定
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAXANISOTROPY, maxAniso);        // 異方性フィルタ
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); // 異方性フィルタ
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC); // 異方性フィルタ

		// ミップマップの使用（距離に応じた最適なテクスチャ選択）
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MAXMIPLEVEL, 0);                  // 0が最も高解像度
		float lodBias = -0.75f;
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&lodBias); // LODバイアス調整
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);   // ミップマップ補完

		//テクスチャ繰り返し
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(cntTex, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	// テクスチャステージの設定
	for (size_t cntTex = 0; cntTex < 1; cntTex++)
	{
		//カラー
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);   // 掛け算
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);   // テクスチャの色と
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);   // 光を
		//透明度
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);   // 掛け算
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);   // テクスチャの透明度と
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);    // 光を
		//その他
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);                       //テクスチャのインデックス
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE); // テクスチャ変換
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_CURRENT);               // テクスチャステージの結果
	}

	// ステージ 1 以降を無効化
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE); // 基本は1枚
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE); // 基本は1枚
}

//----------------------------
// フォグの設定
//----------------------------
void CRenderer::SetFog(const bool bFog, D3DFOGMODE mode, D3DCOLOR color, const float start, const float end) const
{
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, bFog); // フォグの使用

	if (bFog)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, color);            // フォグの色
		m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, mode);         // フォグの種類
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, BIT_DWORD(start)); // フォグの開始距離
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, BIT_DWORD(end));     // フォグの終了距離
	}
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(0, 0, 0, 0)); // フォグの色をクリア
	}
}

//----------------------------
// バックバッファサイズの取得
//----------------------------
HRESULT CRenderer::GetBackBufferSize(D3DXVECTOR2* size) const
{
	// DirectXのサイズを取得する
	LPDIRECT3DSURFACE9 pBackBuffer = nullptr;
	if (SUCCEEDED(m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer)))
	{// バックバッファの取得
		D3DSURFACE_DESC desc;
		pBackBuffer->GetDesc(&desc);
		size->x = (float)desc.Width;
		size->y = (float)desc.Height;
		pBackBuffer->Release();  // 取得したら解放
		pBackBuffer = nullptr;

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// バックバッファサイズとクライアントサイズの比率を取得
//---------------------------------------------------
HRESULT CRenderer::GetBackBufferToClientRatio(D3DXVECTOR2* ratio) const
{
	// DirectXのサイズを取得する
	D3DXVECTOR2 dxSize;
	if (FAILED(GetBackBufferSize(&dxSize))) return E_FAIL;

	// クライアントサイズを取得する
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // クライアントサイズの取得

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = dxSize.x / clientSize.x;
	ratio->y = dxSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// レンダーターゲットサイズの取得
//----------------------------
HRESULT CRenderer::GetTargetSize(D3DXVECTOR2* size) const
{
	// DirectXのサイズを取得する
	LPDIRECT3DSURFACE9 pTarget = nullptr;
	if (SUCCEEDED(m_pD3DDevice->GetRenderTarget(0, &pTarget)))
	{// バックバッファの取得
		D3DSURFACE_DESC desc;
		pTarget->GetDesc(&desc);
		size->x = (float)desc.Width;
		size->y = (float)desc.Height;
		pTarget->Release();  // 取得したら解放
		pTarget = nullptr;

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// レンダーターゲットサイズとクライアントサイズの比率を取得
//---------------------------------------------------
HRESULT CRenderer::GetTargetToClientRatio(D3DXVECTOR2* ratio) const
{
	// DirectXのサイズを取得する
	D3DXVECTOR2 dxSize;
	if (FAILED(GetTargetSize(&dxSize))) return E_FAIL;

	// クライアントサイズを取得する
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // クライアントサイズの取得

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = dxSize.x / clientSize.x;
	ratio->y = dxSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// ビューポートサイズの取得
//----------------------------
HRESULT CRenderer::GetViewportSize(D3DXVECTOR2* size) const
{
	// DirectXのサイズを取得する
	D3DVIEWPORT9 viewport{};
	if (SUCCEEDED(m_pD3DDevice->GetViewport(&viewport)))
	{// バックバッファの取得
		size->x = (float)viewport.Width;
		size->y = (float)viewport.Height;
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//---------------------------------------------------
// ビューポートとクライアントサイズの比率を取得
//---------------------------------------------------
HRESULT CRenderer::GetViewportToClientRatio(D3DXVECTOR2* ratio) const
{
	// ビューポートのサイズを取得する
	D3DXVECTOR2 viewportSize;
	if (FAILED(GetViewportSize(&viewportSize))) return E_FAIL;

	// クライアントサイズを取得する
	RECT clientRect;
	if (FAILED(CMain::GetClientRect(&clientRect))) return E_FAIL; // クライアントサイズの取得

	D3DXVECTOR2 clientSize;
	clientSize.x = (float)(clientRect.right - clientRect.left);
	clientSize.y = (float)(clientRect.bottom - clientRect.top);
	ratio->x = viewportSize.x / clientSize.x;
	ratio->y = viewportSize.y / clientSize.y;

	return S_OK;
}

//----------------------------
// スクリーンサイズ変更
//----------------------------
HRESULT CRenderer::ReSize(const BOOL bWindow)
{
	if (m_pD3DDevice==nullptr)
	{
		return E_FAIL;
	}

	Sleep:

	// デバイスの状態を確認
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();

	if (hr == D3DERR_DRIVERINTERNALERROR)
	{// ドライバエラー(プロジェクト実行不能)
		return hr;
	}
	else if (hr == D3DERR_DEVICELOST)
	{// デバイスロスト (Reset待ち)
		Sleep(60); // 60ms待機
		goto Sleep; // 再度確認
	}
	else if (hr == D3DERR_DEVICENOTRESET || hr == S_OK)
	{// デバイスロスト(Reset可能)又は正常
		if (FAILED(ChangeTarget(RENDER_TARGET::BackBuffer))) { return E_FAIL; }       // バックバッファに戻しておく

		// ウィンドウモードの設定
		if (m_d3dpp.Windowed != bWindow)
		{
			m_d3dpp.Windowed = bWindow;

			//ディスプレイモード
			D3DDISPLAYMODE d3ddm = {};//ダイレクトXディスプレイモードの変数宣言
			if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			{
				return E_FAIL;
			}

			m_d3dpp.FullScreen_RefreshRateInHz = m_d3dpp.Windowed ? 0 : d3ddm.RefreshRate;
		}

		//バックバッファサイズの変更
		if (m_d3dpp.Windowed)
		{// ウィンドウモード
			m_d3dpp.BackBufferWidth = (UINT)CMain::SCREEN_WIDTH;
			m_d3dpp.BackBufferHeight = (UINT)CMain::SCREEN_HEIGHT;
		}
		else
		{// フルスクリーンモード
			RECT clientRect;
			if (FAILED(CMain::GetClientRect(&clientRect)))return E_FAIL;
			m_d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
			m_d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
		}

		DWORD qualityLevels = 0;
		const D3DMULTISAMPLE_TYPE samples[] =
		{
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
			D3DMULTISAMPLE_NONE
		};

		for (auto sample : samples)
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType
			(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				m_d3dpp.BackBufferFormat, m_d3dpp.Windowed,
				sample, &qualityLevels))
				)
			{
				m_d3dpp.MultiSampleType = sample;
				m_d3dpp.MultiSampleQuality = (sample == D3DMULTISAMPLE_NONE) ? 0 : qualityLevels - 1;
				break;
			}
		}

		// リセット前の破棄
		ResetRelease();

		// デバイスのリセット
		if (FAILED(m_pD3DDevice->Reset(&m_d3dpp)))
		{
			return E_FAIL;
		}

		// デバイスの状態を確認
		if (FAILED(m_pD3DDevice->TestCooperativeLevel()))
		{
			return E_FAIL;
		}

		// デバイスのリセット後に描画設定を再度行う
		SetRender();

		// リセット後の再生成
		ResetCreate();

		return S_OK;
	}
	else
	{// その他のエラー
		return hr;
	}
}

//----------------------------
// カーソル位置の取得
//----------------------------
D3DXVECTOR2 CRenderer::GetCursorPos(void) const
{
	POINT cursorPint = CMain::GetCursorPoint();

	// クライアント座標とレンダーターゲットサイズの比率
	D3DXVECTOR2 ratio{};
	if (FAILED(GetTargetToClientRatio(&ratio))) return D3DXVECTOR2(~0, ~0);

	// レンダーターゲット内での位置
	D3DXVECTOR2 cursorPos{};
	cursorPos.x = static_cast<float>(cursorPint.x) * ratio.x;
	cursorPos.y = static_cast<float>(cursorPint.y) * ratio.y;

	// レンダーターゲットサイズ
	D3DXVECTOR2 screenSize{};
	if (FAILED(GetTargetSize(&screenSize))) return D3DXVECTOR2(~0, ~0);

	// レンダーターゲット内での位置の割合
	cursorPos.x /= screenSize.x;
	cursorPos.y /= screenSize.y;

	return cursorPos;
}

//----------------------------
// カーソル位置の取得
//----------------------------
HRESULT CRenderer::GetCursorPos(D3DXVECTOR2* pOutpos) const
{
	POINT cursorPint = CMain::GetCursorPoint();

	// クライアント座標とレンダーターゲットサイズの比率
	D3DXVECTOR2 ratio{};
	if (FAILED(GetTargetToClientRatio(&ratio))) return E_FAIL;

	// レンダーターゲット内での位置
	D3DXVECTOR2 cursorPos{};
	pOutpos->x = static_cast<float>(cursorPint.x) * ratio.x;
	pOutpos->y = static_cast<float>(cursorPint.y) * ratio.y;

	// レンダーターゲットサイズ
	D3DXVECTOR2 screenSize{};
	if (FAILED(GetTargetSize(&screenSize))) return E_FAIL;

	// レンダーターゲット内での位置の割合
	pOutpos->x /= screenSize.x;
	pOutpos->y /= screenSize.y;

	return S_OK;
}

//----------------------------
// レンダーターゲットの変更
//----------------------------
HRESULT CRenderer::ChangeTarget(RENDER_TARGET target)
{
	// レンダーターゲット
	switch (target)
	{
	case RENDER_TARGET::BackBuffer:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pBackBuffer))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pBackBufferZBuffer))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_backBufferViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Main:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_viewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::BackGround:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pBackGroundRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pBackGroundZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_backGroundViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Char:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pCharRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pCharZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_charViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Effect:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pEffectRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pEffectZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_effectViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::UI:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pUIRender))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pUIZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_UIViewport))) { return E_FAIL; }
		break;
	case RENDER_TARGET::Afterimage:
		if (FAILED(m_pD3DDevice->SetRenderTarget(0, m_pAfterimageRender[0]))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pAfterimageZBuff))) { return E_FAIL; }
		if (FAILED(m_pD3DDevice->SetViewport(&m_afterimageViewport))) { return E_FAIL; }
		break;
	}

	return S_OK;
}

//----------------------------
// レンダーテクスチャの取得
//----------------------------
LPDIRECT3DTEXTURE9 CRenderer::GetTexture(void) const
{
	return m_pTexture;
}

//----------------------------
// リセット前の破棄
//----------------------------
void CRenderer::ResetRelease(void)
{
	// 頂点バッファ
	SAFE_RELEASE(m_pVtx);

	// Zバッファ
	SAFE_RELEASE(m_pAfterimageZBuff);
	SAFE_RELEASE(m_pBackGroundZBuff);
	SAFE_RELEASE(m_pCharZBuff);
	SAFE_RELEASE(m_pEffectZBuff);
	SAFE_RELEASE(m_pUIZBuff);
	SAFE_RELEASE(m_pZBuff);
	SAFE_RELEASE(m_pBackBufferZBuffer);

	// サーフェイス
	SAFE_RELEASE_ARRAY(m_pAfterimageRender);
	SAFE_RELEASE(m_pBackGroundRender);
	SAFE_RELEASE(m_pCharRender);
	SAFE_RELEASE(m_pEffectRender);
	SAFE_RELEASE(m_pUIRender);
	SAFE_RELEASE(m_pRender);
	SAFE_RELEASE(m_pBackBuffer);

	// テクスチャ
	SAFE_RELEASE_ARRAY(m_pAfterimageTexture);
	SAFE_RELEASE(m_pBackGroundTexture);
	SAFE_RELEASE(m_pCharTexture);
	SAFE_RELEASE(m_pEffectTexture);
	SAFE_RELEASE(m_pUITexture);
	SAFE_RELEASE(m_pTexture);

	CDebugProc::Uninit();                  // デバック表示
	CManager::GetGui().InvalidateDevice(); // Gui
	CManager::GetLight()->Uninit();        // ライト
}

//----------------------------
// リセット後の再生成
//----------------------------
HRESULT CRenderer::ResetCreate(void)
{
	CManager::GetLight()->Init();                      // ライト
	CManager::GetGui().CreateDevice();                 // Gui
	if (FAILED(CDebugProc::Init())) { return E_FAIL; } // デバック表示
	if (FAILED(SetTexture())) { return E_FAIL; } 	   // テクスチャ描画

	return S_OK;
}

//--------------------------------
// テクスチャをViewport全体に張る
//--------------------------------
HRESULT CRenderer::RenderTexture(LPDIRECT3DTEXTURE9 pTexture, D3DCOLOR color, bool blendOne, bool zEnable, bool bAlphaTest, bool bPoint)
{
	if (pTexture == nullptr) return S_OK; // テクスチャがnullの時は何も描画しないのが正しい

	Vector2 size{};
	GetViewportSize(&size);
	array<DirectX2D::Vertex, 4> vertex;
	for (Index8 cntVtx = 0; cntVtx < 4; cntVtx++)
	{
		vertex[cntVtx].pos = D3DXVECTOR3(size.x * float(cntVtx % 2), size.y * float(cntVtx / 2), 0.0f);
		vertex[cntVtx].col = color;
		vertex[cntVtx].tex = D3DXVECTOR2(float(cntVtx % 2), float(cntVtx / 2));
	}

	SetVertex2D(&m_pVtx, vertex);

	// 頂点バッファ
	m_pD3DDevice->SetStreamSource(0, m_pVtx, 0, sizeof(DirectX2D::Vertex));

	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(DirectX2D::FVF_VERTEX);

	m_pD3DDevice->SetTexture(0, pTexture);

	SetRender(); // 通常描画
	// 残像部分：加算ブレンド
	if (blendOne)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, zEnable);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, bAlphaTest);
	if (bPoint)
	{
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}
	// ポリゴンの描画
	HRESULT hr = m_pD3DDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,//タイプ
		0,                  // インデックス
		2                   //ポリゴン数
	);
	SetRender(); // 元に戻す
	return hr;
}

//----------------------------
// リセット後の再生成
//----------------------------
HRESULT CRenderer::SetTexture(void)
{
	// バックバッファのサイズ
	Vector2 backBufferSize{};
	GetBackBufferSize(&backBufferSize);

	// 描画用ポリゴンの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer
	(
		sizeof(DirectX2D::Vertex) * VT_DEF,
		D3DUSAGE_WRITEONLY,
		DirectX2D::FVF_VERTEX,
		D3DPOOL_DEFAULT,
		&m_pVtx,
		nullptr
	)))
	{
		return E_FAIL;
	}

	// 描画用ポリゴン設定
	array<DirectX2D::Vertex, 4> vertex;
	for (Index8 cntVtx = 0; cntVtx < 4; cntVtx++)
	{
		vertex[cntVtx].pos = D3DXVECTOR3(backBufferSize.x * float(cntVtx % 2), backBufferSize.y * float(cntVtx / 2), 0.0f);
		vertex[cntVtx].tex = D3DXVECTOR2(float(cntVtx % 2), float(cntVtx / 2));
	}
	SetVertex2D(&m_pVtx, vertex);

	// バックバッファ情報を取得する
	if (FAILED(m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))) return E_FAIL;
	if (FAILED(m_pD3DDevice->GetDepthStencilSurface(&m_pBackBufferZBuffer))) return E_FAIL;
	if (FAILED(m_pD3DDevice->GetViewport(&m_backBufferViewport))) return E_FAIL;

	m_pD3DDevice->ColorFill(m_pBackBuffer, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	//-----------------
	// メインテクスチャ
	//-----------------
	// texture生成
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface取得
	m_pTexture->GetSurfaceLevel(0, &m_pRender);
	m_pD3DDevice->ColorFill(m_pRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_viewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	//-----------------
	// 背景描画テクスチャ
	//-----------------
	// texture生成
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pBackGroundTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface取得
	m_pBackGroundTexture->GetSurfaceLevel(0, &m_pBackGroundRender);
	m_pD3DDevice->ColorFill(m_pBackGroundRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pBackGroundZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_backGroundViewport.X = 0;
	m_backGroundViewport.Y = 0;
	m_backGroundViewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_backGroundViewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_backGroundViewport.MinZ = 0.0f;
	m_backGroundViewport.MaxZ = 1.0f;

	//-----------------
    // キャラクター描画テクスチャ
    //-----------------
    // texture生成
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pCharTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface取得
	m_pCharTexture->GetSurfaceLevel(0, &m_pCharRender);
	m_pD3DDevice->ColorFill(m_pCharRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pCharZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_charViewport.X = 0;
	m_charViewport.Y = 0;
	m_charViewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_charViewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_charViewport.MinZ = 0.0f;
	m_charViewport.MaxZ = 1.0f;

	//-----------------
    // エフェクト描画テクスチャ
    //-----------------
    // texture生成
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pEffectTexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface取得
	m_pEffectTexture->GetSurfaceLevel(0, &m_pEffectRender);
	m_pD3DDevice->ColorFill(m_pEffectRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pEffectZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_effectViewport.X = 0;
	m_effectViewport.Y = 0;
	m_effectViewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_effectViewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_effectViewport.MinZ = 0.0f;
	m_effectViewport.MaxZ = 1.0f;

	//-----------------
    // UI描画テクスチャ
    //-----------------
    // texture生成
	if (FAILED(m_pD3DDevice->CreateTexture
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pUITexture, nullptr
	)))
	{
		return E_FAIL;
	}

	// Surface取得
	m_pUITexture->GetSurfaceLevel(0, &m_pUIRender);
	m_pD3DDevice->ColorFill(m_pUIRender, nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pUIZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_UIViewport.X = 0;
	m_UIViewport.Y = 0;
	m_UIViewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_UIViewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_UIViewport.MinZ = 0.0f;
	m_UIViewport.MaxZ = 1.0f;

	//-----------------
	// 残像描画テクスチャ
	//-----------------
	for (Index8 cnt = 0; cnt < m_pAfterimageTexture.size(); ++cnt)
	{
		// texture生成
		if (FAILED(m_pD3DDevice->CreateTexture
		(
			UINT(backBufferSize.x), UINT(backBufferSize.y),
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pAfterimageTexture[cnt], nullptr
		)))
		{
			return E_FAIL;
		}

		// Surface取得
		m_pAfterimageTexture[cnt]->GetSurfaceLevel(0, &m_pAfterimageRender[cnt]);

		m_pD3DDevice->ColorFill(m_pAfterimageRender[cnt], nullptr, D3DCOLOR_RGBA(0, 0, 0, 0));
	}

	// Zバッファ生成
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface
	(
		UINT(backBufferSize.x), UINT(backBufferSize.y),
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE, 0,
		TRUE,
		&m_pAfterimageZBuff, nullptr
	)))
	{
		return E_FAIL;
	}

	// ビューポートの各パラメータを自分で設定する
	m_afterimageViewport.X = 0;
	m_afterimageViewport.Y = 0;
	m_afterimageViewport.Width = DWORD(backBufferSize.x);  // テクスチャの幅
	m_afterimageViewport.Height = DWORD(backBufferSize.y); // テクスチャの高さ
	m_afterimageViewport.MinZ = 0.0f;
	m_afterimageViewport.MaxZ = 1.0f;

	return S_OK;
}