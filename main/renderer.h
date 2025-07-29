//------------------------------------------
//
//描画用の処理の定義・宣言[renderer.h]
//Author: fuma sato
//
//------------------------------------------
#pragma once
#include <array>
#include <span>
#include <string_view>
#include <cmath>
#include "common.h"

//----------------------------
// 描画関係
//----------------------------
namespace renderer
{
	// レンダーターゲット
	enum class RENDER_TARGET : common::Index8
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

    struct Vector3
    {
        float x, y, z;

        Vector3() : x{}, y{}, z{} {}
        Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
        ~Vector3() = default;

        inline float Length(void) const
        {
            return sqrt(x * x + y * y + z * z);
        }

        inline Vector3 Normalize(void) const
        {
            float len = Length();
            if (len == 0.0f) return Vector3(0.0f, 0.0f, 0.0f);
            return Vector3(x / len, y / len, z / len);
        }
    };

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

        Matrix GetMatrix(void)
        {
            D3DXMATRIX mtxRot, mtxTrans, mtxScale;
            D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);        // 平行移動行列の生成
            D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z); // 回転行列の生成
            D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);      // スケーリング行列の生成
            mtxWorld = mtxScale * mtxRot * mtxTrans;                      // マトリックスの合成
            return mtxWorld; // 合成したマトリックスを返す
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

    namespace _2D
    {
        // 頂点
#ifdef USE_DIRECTX9
        constexpr DWORD FVF_VERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット
#else
#endif // USE_DIRECTX9

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

    namespace _3D
    {
        // 頂点
#ifdef USE_DIRECTX9
        constexpr DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1; // 頂点フォーマット
#else
#endif // USE_DIRECTX9

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
    inline constexpr Color White(float alpha = 1.0f) { return D3DCOLOR_ARGB(BYTE(float(255) * alpha), 255, 255, 255); }
    inline constexpr Color Black(float alpha = 1.0f) { return D3DCOLOR_ARGB(BYTE(float(255) * alpha), 0, 0, 0); }
    inline constexpr Color Red(float alpha = 1.0f) { return D3DCOLOR_ARGB(BYTE(float(255) * alpha), 255, 0, 0); }
    inline constexpr Color Green(float alpha = 1.0f) { return D3DCOLOR_ARGB(BYTE(float(255) * alpha), 0, 255, 0); }
    inline constexpr Color Blue(float alpha = 1.0f) { return D3DCOLOR_ARGB(BYTE(float(255) * alpha), 0, 0, 255); }
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
		m_pAfterimageTexture{}, m_pAfterimageRender{}, m_pAfterimageZBuff{}, m_afterimageViewport{},
        m_pFont{}, m_color{}, m_flag{} {}
	~CRenderer() = default;

    CRenderer(const CRenderer&) = delete;
    CRenderer& operator=(const CRenderer&) = delete;
    CRenderer(const CRenderer&&) = default;
    CRenderer& operator=(const CRenderer&&) = default;

	bool Init(const context::InitContext initContext);
	void Uninit(void);
	void Update(const context::UpdateContext updateContext);
    bool Draw(const context::DrawContext drawContext);
	
    bool SetVertex2D(renderer::VertexBuffer* ppVtxBuffer, std::span<const renderer::_2D::Vertex> vertex, renderer::Vector3 offSet = {0.0f,0.0f,0.0f}, float angle = 0.0f) const;
    bool SetVertex2D(renderer::VertexBuffer* ppVtxBuffer, renderer::Vector2 size, renderer::Vector3 pos = { 0.0f,0.0f,0.0f }, float angle = 0.0f, renderer::Color color = renderer::WHITE, renderer::Vector2 texBlock = { 1.0f,1.0f }) const;

	const renderer::Device& GetDevice(void) const { return m_pD3DDevice; };
	void SetRender(void) const;
	void SetFog(const bool bFog, renderer::FogMode mode = D3DFOG_LINEAR, renderer::Color color = D3DCOLOR_ARGB(255, 255, 255, 255), const float start = 100.0f, const float end = 300.0f) const;
	bool GetBackBufferSize(renderer::Vector2* size) const;
	bool GetBackBufferToClientRatio(renderer::Vector2* ratio) const;
	bool GetTargetSize(renderer::Vector2* size) const;
	bool GetTargetToClientRatio(renderer::Vector2* ratio) const;
	bool GetViewportSize(renderer::Vector2* size) const;
	bool GetViewportToClientRatio(renderer::Vector2* ratio) const;
	bool ReSize(const bool bWindow);

    renderer::Vector2 GetCursorPos(void) const;
    bool GetCursorPos(renderer::Vector2* pOutpos) const;

    bool ChangeTarget(renderer::RENDER_TARGET target);
	renderer::Texture GetTexture(void) const;

    bool CreateViewportVertexBuffer(renderer::VertexBuffer* ppVtxBuffer, renderer::Color color) const;

    bool CreatePrint(void);
    void ReleasePrint(void);
    void DrawPrint(const std::string_view string);
	// 非公開
private:
	void ResetRelease(void);
    bool ResetCreate(void);

    bool RenderTexture(renderer::Texture pTexture, D3DCOLOR color = renderer::WHITE, bool blendOne = false, bool zEnable = true, bool bAlphaTest = false, bool bPoint = false);

    bool SetTexture(void);

	renderer::Object m_pD3D;        // DirectXオブジェクト
	renderer::Device m_pD3DDevice;  // DirectXデバイス

	renderer::PresentParameters m_d3dpp;     // DirectXパラメータ

	// テクスチャ描画用
	renderer::VertexBuffer m_pVtx;            // 描画用ポリゴン

	// バックバッファ
	renderer::Surface m_pBackBuffer;         // バックバッファ
    renderer::Surface m_pBackBufferZBuffer;  // バックバッファのZバッファ
    renderer::Viewport m_backBufferViewport; // 描画範囲を指定するビューポート

	// メインテクスチャ
    renderer::Texture m_pTexture;  // 描画先となるテクスチャ
    renderer::Surface m_pRender;   // 描画を行うための表面（サーフェス）
    renderer::Surface m_pZBuff;    // Zバッファ
    renderer::Viewport m_viewport; // 描画範囲を指定するビューポート

	// 背景テクスチャ
    renderer::Texture m_pBackGroundTexture;  // 描画先となるテクスチャ
    renderer::Surface m_pBackGroundRender;   // 描画を行うための表面（サーフェス）
    renderer::Surface m_pBackGroundZBuff;    // Zバッファ
    renderer::Viewport m_backGroundViewport; // 描画範囲を指定するビューポート

	// キャラクターテクスチャ
    renderer::Texture m_pCharTexture;  // 描画先となるテクスチャ
    renderer::Surface m_pCharRender;   // 描画を行うための表面（サーフェス）
    renderer::Surface m_pCharZBuff;    // Zバッファ
    renderer::Viewport m_charViewport; // 描画範囲を指定するビューポート

	// エフェクトテクスチャ
    renderer::Texture m_pEffectTexture;  // 描画先となるテクスチャ
    renderer::Surface m_pEffectRender;   // 描画を行うための表面（サーフェス）
    renderer::Surface m_pEffectZBuff;    // Zバッファ
    renderer::Viewport m_effectViewport; // 描画範囲を指定するビューポート

	// UIテクスチャ
    renderer::Texture m_pUITexture;  // 描画先となるテクスチャ
    renderer::Surface m_pUIRender;   // 描画を行うための表面（サーフェス）
    renderer::Surface m_pUIZBuff;    // Zバッファ
    renderer::Viewport m_UIViewport; // 描画範囲を指定するビューポート

	// 残像用テクスチャ
	std::array<renderer::Texture, 2> m_pAfterimageTexture; // 描画先となるテクスチャ
    std::array <renderer::Surface, 2> m_pAfterimageRender; // 描画を行うための表面（サーフェス）
    renderer::Surface m_pAfterimageZBuff;                  // Zバッファ
    renderer::Viewport m_afterimageViewport;               // 描画範囲を指定するビューポート

    // デバック表示用
    renderer::Font m_pFont;   // フォントポインタ
    renderer::ColorX m_color; // 文字色
    UINT m_flag;              // DrawText設定
};
