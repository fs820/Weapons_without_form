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
// 各種マクロ
//----------------------------
#define BIT_DWORD(d) (bit_cast<DWORD>(d)) // ビットをDWORD型に変換するマクロ

//----------------------------
// 描画関係
//----------------------------
namespace renderer
{
	// レンダーターゲット
	enum class RENDER_TARGET : Index8
	{
		BackBuffer,
		Main,
		BackGround,
		Char,
		Effect,
		UI,
		Afterimage,
		Max
	};
}

//----------------------------
// DirectX定数
//----------------------------
namespace DirectX
{
	// 名前の置き換え
	using Vector2 = D3DXVECTOR2;
	using Vector3 = D3DXVECTOR3;
	using Vector4 = D3DXVECTOR4;
	using Matrix = D3DXMATRIX;
	using Color = D3DCOLOR;
	using ColorX = D3DXCOLOR;

	// トランスフォーム構造体
	struct Transform
	{
		Vector3 pos;     // 位置
		Vector3 rot;     // 回転
		Vector3 scale;   // スケール
		Matrix mtxWorld; // マトリックス

		Transform() : pos{ 0.0f, 0.0f, 0.0f }, rot{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f }, mtxWorld{} {}
		Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : pos(position), rot(rotation), scale(scale), mtxWorld{} {}
		~Transform() = default;

		void Set(const Vector3& pos, const Vector3& rot, const Vector3& scale)
		{
			this->pos = pos;
			this->rot = rot;
			this->scale = scale;
			D3DXMatrixIdentity(&mtxWorld); // マトリックスの初期化
		}

		void Set(const Transform& transform)
		{
			pos = transform.pos;
			rot = transform.rot;
			scale = transform.scale;
			mtxWorld = transform.mtxWorld; // マトリックスのコピー
		}

		void SetMatrix(void)
		{
			D3DXMATRIX mtxRot, mtxTrans, mtxScale;
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);        // 平行移動行列の生成
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z); // 回転行列の生成
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);      // スケーリング行列の生成
			mtxWorld = mtxScale * mtxRot * mtxTrans;                      // マトリックスの合成
		}

		Transform& operator=(const Transform& rhs)
		{
			if (this != &rhs)
			{
				Set(rhs);
			}
			return *this;
		}
		Transform& operator+=(const Transform& rhs)
		{
			pos += rhs.pos;
			rot += rhs.rot;
			scale += rhs.scale;
			return *this;
		}
		Transform& operator-=(const Transform& rhs)
		{
			pos -= rhs.pos;
			rot -= rhs.rot;
			scale -= rhs.scale;
			return *this;
		}
	};

	// 頂点
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

	// 白
	inline constexpr Color White(float alpha = 1.0f) { return D3DCOLOR_ARGB(255, 255, 255, Index8(float(255) * alpha)); }
}

//----------------------------
// DirectX2D定数
//----------------------------
 namespace DirectX2D
{
	using namespace DirectX; // DirectXの名前空間を使用

	// 頂点
	constexpr DWORD FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット

	// 頂点情報の構造体の定義
	struct Vertex
	{
		Vector3 pos; // 座標
		float rhw;   // 座標変換係数(1.0f)
		Color col;   // カラー
		Vector2 tex; // テクスチャ座標

		Vertex() : pos{ 0.0f,0.0f,0.0f }, rhw{ 1.0f }, col{ WHITE }, tex{ 0.0f,0.0f } {}
		~Vertex() = default;
	};
}

 //----------------------------
// DirectX3D定数
//----------------------------
 namespace DirectX3D
 {
	 using namespace DirectX; // DirectXの名前空間を使用

	 // 頂点
	 constexpr DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット

	 // 頂点情報の構造体の定義
	 struct Vertex
	 {
		 Vector3 pos; // 座標
		 Vector3 nor; // 法線ベクトル
		 Color col;   // カラー
		 Vector2 tex; // テクスチャ座標

		 Vertex() : pos{ 0.0f,0.0f,0.0f }, nor{ 0.0f,0.0f,0.0f }, col{ WHITE }, tex{ 0.0f,0.0f } {}
		 ~Vertex() = default;
	 };
 }

//------------------
// レンダラークラス
//------------------
class CRenderer
{
// 公開
public:
	CRenderer() : m_pD3D{}, m_pD3DDevice{}, m_d3dpp{},
		m_pVtx{}, m_pBackBuffer{}, m_pBackBufferZBuffer{}, m_backBufferViewport{},
		m_pTexture{}, m_pRender{}, m_pZBuff{}, m_viewport{},
		m_pBackGroundTexture{}, m_pBackGroundRender{}, m_pBackGroundZBuff{}, m_backGroundViewport{},
		m_pCharTexture{}, m_pCharRender{}, m_pCharZBuff{}, m_charViewport{},
		m_pEffectTexture{}, m_pEffectRender{}, m_pEffectZBuff{}, m_effectViewport{},
		m_pUITexture{}, m_pUIRender{}, m_pUIZBuff{}, m_UIViewport{},
		m_pAfterimageTexture{}, m_pAfterimageRender{}, m_pAfterimageZBuff{}, m_afterimageViewport{} {}
	~CRenderer() = default;

	HRESULT Init(HINSTANCE hInstanse, HWND hWnd, const BOOL bWindow = TRUE);
	void Uninit(void);
	void Update(void) const;
	HRESULT Draw(void);
	
	HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, span<const DirectX2D::Vertex> vertex, DirectX::Vector3 offSet = {0.0f,0.0f,0.0f}, float angle = 0.0f);
	HRESULT SetVertex2D(LPDIRECT3DVERTEXBUFFER9* ppVtxBuffer, DirectX::Vector2 size, DirectX::Vector3 pos = { 0.0f,0.0f,0.0f }, float angle = 0.0f, DirectX::Color color = DirectX::WHITE, DirectX::Vector2 texBlock = { 1.0f,1.0f });

	const LPDIRECT3DDEVICE9& GetDevice(void) const { return m_pD3DDevice; };
	void SetRender(void) const;
	void SetFog(const bool bFog, D3DFOGMODE mode = D3DFOG_LINEAR, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255), const float start = 100.0f, const float end = 300.0f) const;
	HRESULT GetBackBufferSize(D3DXVECTOR2* size) const;
	HRESULT GetBackBufferToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT GetTargetSize(D3DXVECTOR2* size) const;
	HRESULT GetTargetToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT GetViewportSize(D3DXVECTOR2* size) const;
	HRESULT GetViewportToClientRatio(D3DXVECTOR2* ratio) const;
	HRESULT ReSize(const BOOL bWindow);

	D3DXVECTOR2 GetCursorPos(void) const;
	HRESULT GetCursorPos(D3DXVECTOR2* pOutpos) const;

	HRESULT ChangeTarget(renderer::RENDER_TARGET target);
	LPDIRECT3DTEXTURE9 GetTexture(void) const;
	// 非公開
private:
	void ResetRelease(void);
	HRESULT ResetCreate(void);

	HRESULT RenderTexture(LPDIRECT3DTEXTURE9 pTexture, D3DCOLOR color = DirectX::WHITE, bool blendOne = false, bool zEnable = true, bool bAlphaTest = false, bool bPoint = false);

	HRESULT SetTexture(void);

	LPDIRECT3D9 m_pD3D;              // DirectXオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;  // DirectXデバイス

	D3DPRESENT_PARAMETERS m_d3dpp;     // DirectXパラメータ

	// テクスチャ描画用
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;            // 描画用ポリゴン

	// バックバッファ
	LPDIRECT3DSURFACE9 m_pBackBuffer;        // バックバッファ
	LPDIRECT3DSURFACE9 m_pBackBufferZBuffer; // バックバッファのZバッファ
	D3DVIEWPORT9       m_backBufferViewport; // 描画範囲を指定するビューポート

	// メインテクスチャ
	LPDIRECT3DTEXTURE9 m_pTexture; // 描画先となるテクスチャ
	LPDIRECT3DSURFACE9 m_pRender;  // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pZBuff;   // Zバッファ
	D3DVIEWPORT9       m_viewport; // 描画範囲を指定するビューポート

	// 背景テクスチャ
	LPDIRECT3DTEXTURE9 m_pBackGroundTexture; // 描画先となるテクスチャ
	LPDIRECT3DSURFACE9 m_pBackGroundRender;  // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pBackGroundZBuff;   // Zバッファ
	D3DVIEWPORT9       m_backGroundViewport; // 描画範囲を指定するビューポート

	// キャラクターテクスチャ
	LPDIRECT3DTEXTURE9 m_pCharTexture; // 描画先となるテクスチャ
	LPDIRECT3DSURFACE9 m_pCharRender;  // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pCharZBuff;   // Zバッファ
	D3DVIEWPORT9       m_charViewport; // 描画範囲を指定するビューポート

	// エフェクトテクスチャ
	LPDIRECT3DTEXTURE9 m_pEffectTexture; // 描画先となるテクスチャ
	LPDIRECT3DSURFACE9 m_pEffectRender;  // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pEffectZBuff;   // Zバッファ
	D3DVIEWPORT9       m_effectViewport; // 描画範囲を指定するビューポート

	// UIテクスチャ
	LPDIRECT3DTEXTURE9 m_pUITexture; // 描画先となるテクスチャ
	LPDIRECT3DSURFACE9 m_pUIRender;  // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pUIZBuff;   // Zバッファ
	D3DVIEWPORT9       m_UIViewport; // 描画範囲を指定するビューポート

	// 残像用テクスチャ
	array<LPDIRECT3DTEXTURE9, 2> m_pAfterimageTexture; // 描画先となるテクスチャ
	array <LPDIRECT3DSURFACE9, 2> m_pAfterimageRender; // 描画を行うための表面（サーフェス）
	LPDIRECT3DSURFACE9 m_pAfterimageZBuff;             // Zバッファ
	D3DVIEWPORT9       m_afterimageViewport;           // 描画範囲を指定するビューポート
};