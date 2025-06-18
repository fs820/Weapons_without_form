//------------------------------------------
//
// ImGui処理 [gui.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "gui.h"
#include "manager.h"
#include "math.h"
#include "debug.h"

//---------------------------------------
//
// ImGui
//
//---------------------------------------

//------------------------------
// 初期化処理
//------------------------------
void CGui::Init(void) const
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// ImGuiコンテキストの作成
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO(); (void)io;

	// スタイル設定
	StyleColorsDark();

	// プラットフォーム/レンダラーバックエンドの初期化
	ImGui_ImplWin32_Init(CMain::GethWnd());
	ImGui_ImplDX9_Init(pDevice);
}

//------------------------------
// 終了処理
//------------------------------
void CGui::Uninit(void) const
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

//------------------------------
// 終了処理
//------------------------------
void CGui::Update(void) const
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	Begin("Hello, world!");
	Text("This is some useful text.");
	static float f = 0.0f;
	SliderFloat("float", &f, 0.0f, 1.0f);
	if (Button("Button"))
	{// ボタンが押されたら
		
	}
	End();

	Render();
}

//------------------------------
// 描画処理
//------------------------------
void CGui::Draw(void) const
{
	ImGui_ImplDX9_RenderDrawData(GetDrawData());
}