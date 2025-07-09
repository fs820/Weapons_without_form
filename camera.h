//---------------------------------------
//
//�J�����̒�`�E�錾[camera.h]
//Author fuma sato
//
//---------------------------------------
#pragma once
#include "main.h"
#include "renderer.h"

// �J�������
namespace camera
{
	static constexpr int CAMERA_XNUM = 1;                      //�J�����̐�X
	static constexpr int CAMERA_YNUM = 1;                      //�J�����̐�Y
	static constexpr float CAMERA_VIEW_SPEED = 0.01f;          //��]�X�s�[�h
	static constexpr float CAMERA_ROT_SPEED = 0.05f;           //��]�X�s�[�h
	static constexpr float CAMERA_SPEED = 2.0f;                //�ړ��X�s�[�h
	static constexpr float CAMERA_INA = 0.05f;                 //�Ǐ]�{��
	static constexpr float CAMERA_ROTX = 0.2f;                 //�c��]�͈�
	static constexpr float CAMERA_ROTX_GRA = 0.01f;            //�c��]�n�ʃX�y�[�X
	static constexpr float CAMERA_DISTANCE_SPEED = 5.0f;       //�Y�[���X�s�[�h
	static constexpr float CAMERA_DISTANCE_MIN = 5.0f;         //�Y�[������
	static constexpr float CAMERA_DISTANCE_MAX = 100.0f;       //�Y�[������
	static constexpr float CAMERA_DISTANCE_VIEW_MAX = 1000.0f; //�Y�[������
	static constexpr float CAMERA_WIDTH = 45.0f;               //����̍L��
	static constexpr float CAMERA_MIN = 5.0f;                  //����̍ŏ�
	static constexpr float CAMERA_MAX = 500.0f;                //����̍ő�

	//�J�����\����
	struct Info
	{
		D3DXVECTOR3 posV;
		D3DXVECTOR3 posVDest;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 posRDest;
		D3DXVECTOR3 posU;
		D3DXVECTOR3 rot;
		D3DXMATRIX mtxProjection;
		D3DXMATRIX mtxView;
		float fDistance;
		float fDistanceMin;
		float fDistanceMax;
	};
}

//-------------------
// �J�����N���X
//-------------------
class CCamera final
{
	// ���J
public:
	CCamera() : m_camera{} {}
	~CCamera() = default;

	void Init(void);     // ����������
	void Uninit(void);   // �I������
	void Update(void);   // �X�V����
	void Set(Index idx); // �`�揈��
	D3DXVECTOR3 GetPos(Index idx) const; //�J�����̈ʒu���擾
	void View(Index idx);
	void Game(Index idx);

	size_t GetCameraNum(void) const { return m_camera.size(); }

	// ����J
private:
	array<camera::Info, camera::CAMERA_XNUM * camera::CAMERA_YNUM> m_camera;
};