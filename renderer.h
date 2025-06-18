//------------------------------------------
//
//描画用の処理の定義・宣言[renderer.h]
//Author: fuma sato
//
//------------------------------------------
#pragma once
#include "main.h"

//----------------------------
// DirectXライブラリ
//----------------------------
#include "d3dx9.h"

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")

//----------------------------
// DirectX2D定数
//----------------------------
 namespace DirectX2D
{
	 // 名前の置き換え
	 using Vector2 = D3DXVECTOR2;
	 using Vector3 = D3DXVECTOR3;
	 using Vector4 = D3DXVECTOR4;
	 using Color = D3DCOLOR;
	 using ColorX = D3DXCOLOR;

	// 頂点
	constexpr DWORD FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット
	constexpr int VT_DEF = 4;// 基本頂点数

	// 色の定数
	constexpr Color WHITE = D3DCOLOR_ARGB(255, 255, 255, 255); // 白
	constexpr Color RED = D3DCOLOR_ARGB(255, 255, 0, 0);       // 赤
	constexpr Color GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);     // 緑
	constexpr Color BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);      // 青
	constexpr Color BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);       // 黒

	// 半透明 translucent
	constexpr Color TRANSLUCENT_WHITE = D3DCOLOR_ARGB(128, 255, 255, 255); // 白
	constexpr Color TRANSLUCENT_RED = D3DCOLOR_ARGB(128, 255, 0, 0);       // 赤
	constexpr Color TRANSLUCENT_GREEN = D3DCOLOR_ARGB(128, 0, 255, 0);     // 緑
	constexpr Color TRANSLUCENT_BLUE = D3DCOLOR_ARGB(128, 0, 0, 255);      // 青
	constexpr Color TRANSLUCENT_BLACK = D3DCOLOR_ARGB(128, 0, 0, 0);       // 黒

	// 透明 transparency
	constexpr Color TRANSPARENCY_WHITE = D3DCOLOR_ARGB(0, 255, 255, 255); // 白
	constexpr Color TRANSPARENCY_RED = D3DCOLOR_ARGB(0, 255, 0, 0);       // 赤
	constexpr Color TRANSPARENCY_GREEN = D3DCOLOR_ARGB(0, 0, 255, 0);     // 緑
	constexpr Color TRANSPARENCY_BLUE = D3DCOLOR_ARGB(0, 0, 0, 255);      // 青
	constexpr Color TRANSPARENCY_BLACK = D3DCOLOR_ARGB(0, 0, 0, 0);       // 黒

	// 頂点情報の構造体の定義
	using Vertex = struct
	{
		Vector3 pos; // 座標
		float rhw;   // 座標変換係数(1.0f)
		Color col;   // カラー
		Vector2 tex; // テクスチャ座標
	};
}

 //----------------------------
// DirectX3D定数
//----------------------------
 namespace DirectX3D
 {
	 // 名前の置き換え
	 using Vector2 = D3DXVECTOR2;
	 using Vector3 = D3DXVECTOR3;
	 using Vector4 = D3DXVECTOR4;
	 using Color = D3DCOLOR;
	 using ColorX = D3DXCOLOR;

	 // 頂点
	 constexpr DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット
	 constexpr int VT_DEF = 4;// 基本頂点数

	 // 色の定数
	 constexpr Color WHITE = D3DCOLOR_ARGB(255, 255, 255, 255); // 白
	 constexpr Color RED = D3DCOLOR_ARGB(255, 255, 0, 0);       // 赤
	 constexpr Color GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);     // 緑
	 constexpr Color BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);      // 青
	 constexpr Color BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);       // 黒

	 // 半透明 translucent
	 constexpr Color TRANSLUCENT_WHITE = D3DCOLOR_ARGB(128, 255, 255, 255); // 白
	 constexpr Color TRANSLUCENT_RED = D3DCOLOR_ARGB(128, 255, 0, 0);       // 赤
	 constexpr Color TRANSLUCENT_GREEN = D3DCOLOR_ARGB(128, 0, 255, 0);     // 緑
	 constexpr Color TRANSLUCENT_BLUE = D3DCOLOR_ARGB(128, 0, 0, 255);      // 青
	 constexpr Color TRANSLUCENT_BLACK = D3DCOLOR_ARGB(128, 0, 0, 0);       // 黒

	 // 透明 transparency
	 constexpr Color TRANSPARENCY_WHITE = D3DCOLOR_ARGB(0, 255, 255, 255); // 白
	 constexpr Color TRANSPARENCY_RED = D3DCOLOR_ARGB(0, 255, 0, 0);       // 赤
	 constexpr Color TRANSPARENCY_GREEN = D3DCOLOR_ARGB(0, 0, 255, 0);     // 緑
	 constexpr Color TRANSPARENCY_BLUE = D3DCOLOR_ARGB(0, 0, 0, 255);      // 青
	 constexpr Color TRANSPARENCY_BLACK = D3DCOLOR_ARGB(0, 0, 0, 0);       // 黒

	 // 頂点情報の構造体の定義
	 using Vertex = struct
	 {
		 Vector3 pos; // 座標
		 Vector3 nor; // 法線ベクトル
		 Color col;   // カラー
		 Vector2 tex; // テクスチャ座標
	 };
 }

//------------------
// レンダラークラス
//------------------
class CRenderer
{
// 公開
public:
	CRenderer() : m_pD3D{}, m_pD3DDevice{}, m_d3dpp{} {}
	~CRenderer() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow = TRUE);
	void Uninit(void);
	void Update(void) const;
	void Draw(void) const;

	const LPDIRECT3DDEVICE9& GetDevice(void) const { return m_pD3DDevice; };
	void SetRender(void) const;
	HRESULT GetDxScreenSize(D3DXVECTOR2* size) const;
	HRESULT GetDxScreenToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT ReSize(const BOOL bWindow);

	D3DXVECTOR2 GetCursorPos(void) const;
	HRESULT GetCursorPos(D3DXVECTOR2* pOutpos) const;
	// 非公開
private:
	void ResetRelease(void) const;
	void ResetCreate(void) const;

	LPDIRECT3D9 m_pD3D;              // DirectXオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;  // DirectXデバイス

	D3DPRESENT_PARAMETERS m_d3dpp;     // DirectXパラメータ
};