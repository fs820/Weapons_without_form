//------------------------------------------
//
// ImGui���� [gui.cpp]
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
// ����������
//------------------------------
void CGui::Init(void) const
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// ImGui�R���e�L�X�g�̍쐬
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO(); (void)io;

	// �X�^�C���ݒ�
	StyleColorsDark();

	// �v���b�g�t�H�[��/�����_���[�o�b�N�G���h�̏�����
	ImGui_ImplWin32_Init(CMain::GethWnd());
	ImGui_ImplDX9_Init(pDevice);
}

//------------------------------
// �I������
//------------------------------
void CGui::Uninit(void) const
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

//------------------------------
// �I������
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
	{// �{�^���������ꂽ��
		
	}
	End();

	Render();
}

//------------------------------
// �`�揈��
//------------------------------
void CGui::Draw(void) const
{
	ImGui_ImplDX9_RenderDrawData(GetDrawData());
}