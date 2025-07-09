//---------------------------------------
//
//���͏���[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include"input.h"

using namespace input; // �C���v�b�g��Ԃ̎g�p

//#if 0

//----------------------------------
//
// �C���v�b�g�N���X (�Ăяo���p����)
//
//----------------------------------

// �ÓI�����o�ϐ��̒�`
CInput CInput::m_instance{}; // ���g�̃C���X�^���X

//-----------------
// ������
//-----------------
HRESULT CInput::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// �L�[�{�[�h
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboardManager;

		if (m_pKeyboard != nullptr)
		{
			if (FAILED(m_pKeyboard->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// �}�E�X
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouseManager;

		if (m_pMouse != nullptr)
		{
			if (FAILED(m_pMouse->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	// �R���g���[���[
	if (m_pController == nullptr)
	{
		m_pController = new CInputControllerManager;

		if (m_pController != nullptr)
		{
			if (FAILED(m_pController->Init(hInstanse, hWnd))) return E_FAIL;
		}
	}

	return S_OK;
}

//-----------------
// �I��
//-----------------
void CInput::Uninit(void)
{
	// �L�[�{�[�h
	SAFE_UNINIT(m_pKeyboard);

	// �}�E�X
	SAFE_UNINIT(m_pMouse);

	// �R���g���[���[
	SAFE_UNINIT(m_pController);
}

//-----------------
// �X�V
//-----------------
void CInput::Update(void)
{
	// �L�[�{�[�h
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// �}�E�X
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	// �R���g���[���[
	if (m_pController != nullptr)
	{
		m_pController->Update();
	}
}

//-----------------
// �����Ă���
//-----------------
bool CInput::IsPress(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsPress(idx, button) || m_pMouse->IsPress(idx, button) || m_pController->IsPress(idx, button);
}

//-----------------
// �������u��
//-----------------
bool CInput::IsTrigger(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsTrigger(idx, button) || m_pMouse->IsTrigger(idx, button) || m_pController->IsTrigger(idx, button);
}

//-----------------
// �������u��
//-----------------
bool CInput::IsRelease(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRelease(idx, button) || m_pMouse->IsRelease(idx, button) || m_pController->IsRelease(idx, button);
}

//-----------------
// ������
//-----------------
bool CInput::IsRepeat(Index8 idx, BUTTON button)
{
	return m_pKeyboard->IsRepeat(idx, button) || m_pMouse->IsRepeat(idx, button) || m_pController->IsRepeat(idx, button);
}

//-----------------
// ������
//-----------------
size_t CInput::Count(void)
{
	return m_pKeyboard->Count() + m_pMouse->Count() + m_pController->Count();
}

//--------------------------
//
// �L�[�{�[�h
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputKeyboardManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputKeyboard* pKeyboard{}; // �L�[�{�[�h�|�C���^

	// raw
	do
	{// ���[�v
		if (pKeyboard != nullptr) return E_FAIL;  // null
		pKeyboard = new CInputRawInputKeyboard;   // rawInput����
		if (pKeyboard == nullptr)  return E_FAIL; // �������s

		// ������
		if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
		{// ����
			m_apKeyboard.push_back(pKeyboard); // �L�[�{�[�h�ǉ�
			pKeyboard = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pKeyboard); // �̂Ă�
			break;
		}
	} while (true);

	if (m_apKeyboard.size() > 0) { return S_OK; } // �L�[�{�[�h�����ς�

	// Direct
	if (pKeyboard != nullptr) return E_FAIL;   // null
	pKeyboard = new CInputDirectInputKeyboard; // DirectInput����
	if (pKeyboard == nullptr)  return E_FAIL;  // �������s

	// ������
	if (SUCCEEDED(pKeyboard->Init(hInstanse, hWnd)))
	{// ����
		m_apKeyboard.push_back(pKeyboard); // �L�[�{�[�h�ǉ�
		return S_OK;
	}
	SAFE_UNINIT(pKeyboard); // �̂Ă�
	return E_FAIL;
}

//----------------------
//�I������
//----------------------
void CInputKeyboardManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apKeyboard); // �L�[�{�[�h�����ׂĔj��
}

//-------------------------
//�X�V����
//-------------------------
void CInputKeyboardManager::Update(void)
{
	for (auto& pKeyboard : m_apKeyboard)
	{// �S�ẴL�[�{�[�h�̍X�V
		pKeyboard->Update();
	}
}

//------------------------------
//�L�[��������
//------------------------------
bool CInputKeyboardManager::IsPress(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool CInputKeyboardManager::IsTrigger(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool CInputKeyboardManager::IsRelease(Index8 idx, BUTTON button)
{
	
}

//------------------------------
//�L�[����������
//------------------------------
bool CInputKeyboardManager::IsRepeat(Index8 idx, BUTTON button)
{
	
}

//--------------------------
//
// �}�E�X
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputMouseManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputMouse* pMouse{}; // �}�E�X�|�C���^

	// raw
	do
	{// ���[�v
		if (pMouse != nullptr) return E_FAIL;  // null
		pMouse = new CInputRawInputMouse;      // rawInput����
		if (pMouse == nullptr)  return E_FAIL; // �������s

		// ������
		if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
		{// ����
			m_apMouse.push_back(pMouse); // �}�E�X�ǉ�
			pMouse = nullptr; // null
		}
		else
		{
			SAFE_UNINIT(pMouse); // �̂Ă�
			break;
		}
	} while (true);

	if (m_apMouse.size() > 0) { return S_OK; } // �}�E�X�����ς�

	// Direct
	if (pMouse != nullptr) return E_FAIL;  // null
	pMouse = new CInputDirectInputMouse;   // DirectInput����
	if (pMouse == nullptr)  return E_FAIL; // �������s

	// ������
	if (SUCCEEDED(pMouse->Init(hInstanse, hWnd)))
	{// ����
		m_apMouse.push_back(pMouse); // �}�E�X�ǉ�
		return S_OK;
	}
	SAFE_UNINIT(pMouse); // �̂Ă�
	return E_FAIL;
}

//----------------------
//�I������
//----------------------
void CInputMouseManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apMouse); // �}�E�X�����ׂĔj��
}

//-------------------------
//�X�V����
//-------------------------
void CInputMouseManager::Update(void)
{
	for (auto& pMouse : m_apMouse)
	{// �S�Ẵ}�E�X�̍X�V
		pMouse->Update();
	}
}

//------------------------------
//�L�[��������
//------------------------------
bool CInputMouseManager::IsPress(Index8 idx, BUTTON button)
{

}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool CInputMouseManager::IsTrigger(Index8 idx, BUTTON button)
{

}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool CInputMouseManager::IsRelease(Index8 idx, BUTTON button)
{

}

//------------------------------
//�L�[����������
//------------------------------
bool CInputMouseManager::IsRepeat(Index8 idx, BUTTON button)
{

}

//--------------
//�}�E�X�ړ�
//--------------
Axis CInputMouseManager::GetAxis(Index8 idx)
{

}

//----------------
//�L�[��������
//----------------
float CInputMouseManager::GetWheel(Index8 idx)
{

}

//------------------------------------
//
// �R���g���[���[�N���X
//
//------------------------------------

//-------------------
// ������
//-------------------
HRESULT CInputControllerManager::Init(HINSTANCE hInstanse, HWND hWnd)
{
	CInputController* pController{}; // �R���g���[���[�|�C���^

	// X
	do
	{// ���[�v
		if (pController != nullptr) return E_FAIL;  // null
		pController = new CInputXInputController;   // XInput����
		if (pController == nullptr)  return E_FAIL; // �������s

		// ������
		if (SUCCEEDED(pController->Init(hInstanse, hWnd)))
		{// ����
			m_apController.push_back(pController); // �R���g���[���[�ǉ�
			pController = nullptr;                 // null
			continue;                              // ������
		}
		SAFE_UNINIT(pController); // �̂Ă�

		// Direct
		if (pController != nullptr) return E_FAIL;     // null
		pController = new CInputDirectInputController; // DirectInput����
		if (pController == nullptr)  return E_FAIL;    // �������s

		// ������
		if (SUCCEEDED(pController->Init(hInstanse, hWnd)))
		{// ����
			m_apController.push_back(pController); // �R���g���[���[�ǉ�
			return S_OK;
		}
		else
		{
			SAFE_UNINIT(pController); // �̂Ă�
			break;
		}
	} while (true);
	return S_OK;
}

//-------------------
// ������
//-------------------
void CInputControllerManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apController); // �R���g���[���[�����ׂĔj��
}

//-------------------
// ������
//-------------------
void CInputControllerManager::Update(void)
{
	for (auto& pController : m_apController)
	{// �S�ẴR���g���[���[�̍X�V
		pController->Update();
	}
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsPress(Index idx, BUTTON button)
{

}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsTrigger(Index idx, BUTTON button)
{

}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsRelease(Index idx, BUTTON button)
{

}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsRepeat(Index idx, BUTTON button)
{

}

//-------------------
// ������
//-------------------
void CInputControllerManager::SetVibrate(Index idx, float fLeftPower, float fReghtPower)
{
	m_apController[idx]->Vibrate(fLeftPower, fReghtPower);
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsXInputControllerConnected(Index idx)
{

}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsDirectInputControllerConnected(Index idx)
{

}

//-------------------
// ������
//-------------------
string_view CInputControllerManager::ControllerName(Index idx)
{

}

//--------------------------
//
// �L�[�{�[�h���
//
//--------------------------

//------
// �X�V
//------
void CInputKeyboard::Update(void)
{
	array <BYTE, input::MAX_KEY> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = (m_state[cntKey] & 0x80) != 0;    // �������Ă��邩�ǂ���
			bool old = (m_stateOld[cntKey] & 0x80) != 0; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // �����[�X

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// �L�[�̕ω�
				// �^�C���̏�����
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// �����Ă���
				// �^�C���̑���
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// ���s�[�g��
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// ���s�[�g�Ԋu
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��
		}
	}
}

//--------------------------
//
// �}�E�X���
//
//--------------------------

//------
// �X�V
//------
void CInputMouse::Update(void)
{
	// �{�^���̏�Ԃ����
	array<bool, Index8(input::MOUSE_BUTTON::Max)> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = m_state[cntKey];    // �������Ă��邩�ǂ���
			bool old = m_stateOld[cntKey]; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // �����[�X

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// �L�[�̕ω�
				// �^�C���̏�����
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// �����Ă���
				// �^�C���̑���
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// ���s�[�g��
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// ���s�[�g�Ԋu
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��
		}
	}

	GetMove(&m_Move); 	    // �����̏�Ԃ����炤
	GetWheel(&m_WheelMove); // �z�C�[���̏�Ԃ����炤
}

//--------------------------
//
// �R���g���[���[���
//
//--------------------------

//------
// �X�V
//------
void CInputController::Update(void)
{
	// �{�^���̏�Ԃ����
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> state{};
	if (SUCCEEDED(GetState(state)))
	{
		for (Index8 cntKey = 0; cntKey < state.size(); cntKey++)
		{
			m_state[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = m_state[cntKey];    // �������Ă��邩�ǂ���
			bool old = m_stateOld[cntKey]; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME) ? true : false; // �����[�X

			if (m_bTrigger[cntKey] || m_bRelease[cntKey])
			{// �L�[�̕ω�
				// �^�C���̏�����
				m_PressTime[cntKey] = 0.0f;
				m_RepeatTime[cntKey] = 0.0f;
			}
			else if (m_bPress[cntKey])
			{// �����Ă���
				// �^�C���̑���
				m_PressTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
				if (m_PressTime[cntKey] >= REPEAT_START_TIME)
				{// ���s�[�g��
					m_RepeatTime[cntKey] += CMain::GetDeltaTimeGameSpeed();
					if (m_RepeatTime[cntKey] >= REPEAT_INTERVAL_TIME)
					{// ���s�[�g�Ԋu
						m_RepeatTime[cntKey] = 0.0f;
						m_bRepeat[cntKey] = now ? true : false;
					}
				}
			}

			m_stateOld[cntKey] = state[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��
		}
	}

	GetStick(m_Stick); 	   // �X�e�B�b�N�̏�Ԃ����炤
	GetTrigger(m_Trigger); // �g���K�[�̏�Ԃ����炤
}

//--------------------------
//
// rawInput���
//
//--------------------------

//---------------------------------------------
// ���b�Z�[�W���������f�[�^���f�o�C�X�ɑ��M
//---------------------------------------------
HRESULT CInputRawInput::AnalysisRawData(RAWINPUT* pRawData)
{

}

//--------------------------
//
// rawInput�L�[�{�[�h
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputRawInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{


	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputRawInputKeyboard::Uninit(void)
{

}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputRawInputKeyboard::GetState(span<BYTE> keyState) const
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //�O��̃L�[����ۑ�
	}

	BYTE KeyState[MAX_KEY];//�ꎞ���͊i�[
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // �������Ă��邩�ǂ���
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // �O�񉟂��Ă������ǂ���

			m_bKeyPress[nCntkey] = now;           // ��
			m_bKeyTrigger[nCntkey] = now && !old; // ����
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // �����[�X

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// �L�[�̕ω�
				// �^�C���̏�����
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// �����Ă���
				// �^�C���̑���
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// ���s�[�g��
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// ���s�[�g�Ԋu
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey] = now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//�A�N�Z�X�����̎擾
	}
}

//--------------------------
//
// rawInput�}�E�X
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputRawInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{

	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputRawInputMouse::Uninit(void)
{

}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputRawInputMouse::GetState(span<bool> buttonState) const
{
	m_MouseStateOld = m_MouseState; //�O��̃}�E�X����ۑ�

	DIMOUSESTATE2 MouseState;//�}�E�X���//�ꎞ���͊i�[
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//�ꎞ�I�ȏ��𐳋K���ɓn��

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // �������Ă��邩�ǂ���
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // �O�񉟂��Ă������ǂ���

			m_bPress[nCntButton] = now;           // ��
			m_bTrigger[nCntButton] = now && !old; // ����
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//�A�N�Z�X�����̎擾
	}
}

//--------------------------
//
//�p�b�h
//
//--------------------------

//--------------------------------
//����������
//--------------------------------
HRESULT CInputXInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{
	XInputEnable(true);
	return S_OK;
}

//------------------------
//�I������
//------------------------
void CInputXInputController::Uninit(void)
{
	XInputEnable(false);
}

//------------------------
//�X�V����
//------------------------
HRESULT CInputXInputController::GetState(span<bool> state) const
{
	XINPUT_STATE XState;

	if (XInputGetState(0, &XState) == ERROR_SUCCESS)
	{
		state = XState;
	}
}
//----------------------------
//�L�[����������
//----------------------------
bool CInputXInputController::IsRepeat(JOYKEY key)
{
	static bool bOldJoy[CONTROLLER_MAX][JOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][JOYKEY_MAX] = { 0 };
	BYTE Trigger = 0;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (key == JOYKEY_LT || key == JOYKEY_RT)
			{
				if (key == JOYKEY_LT)
				{
					Trigger = g_joykeyState[i].Gamepad.bLeftTrigger;
				}
				else
				{
					Trigger = g_joykeyState[i].Gamepad.bRightTrigger;
				}
			}

			if (bOldJoy[i][key] == true && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				return true;
			}
			else if (bOldJoy[i][key] == false && (((g_joykeyState[i].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldJoy[i][key] = true;
				}
			}
			else
			{
				bOldJoy[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (key == JOYKEY_LT || key == JOYKEY_RT)
	{
		if (key == JOYKEY_LT)
		{
			Trigger = g_joykeyState[Controller].Gamepad.bLeftTrigger;
		}
		else
		{
			Trigger = g_joykeyState[Controller].Gamepad.bRightTrigger;
		}
	}

	if (bOldJoy[Controller][key] == true && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		return true;
	}
	else if (bOldJoy[Controller][key] == false && (((g_joykeyState[Controller].Gamepad.wButtons & (0x01 << key)) ? true : false) == true || Trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD))
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldJoy[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldJoy[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
// ���X�e�B�b�N�p�x
//-----------------
float CInputXInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// ���X�e�B�b�N�p�x
//-----------------
float CInputXInputController::GetRightStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// ���X�e�B�b�N�|���
//-----------------
float CInputXInputController::GetLeftStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//-----------------
// ���X�e�B�b�N�|���
//-----------------
float CInputXInputController::GetRightStickLength(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbLX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbLY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_joykeyState[Controller].Gamepad.sThumbRX;
		fStick[1] = (float)g_joykeyState[Controller].Gamepad.sThumbRY;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	return &fStick[0];
}

//----------------
//�U��
//----------------
void CInputXInputController::Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	XINPUT_VIBRATION vibration{ wLeftMotorSpeed ,wRightMotorSpeed };
	XInputSetState(0, &vibration);
}

//----------------------------------
//
// DirectInput���
//
//----------------------------------

LPDIRECTINPUT8 CInputDirectInput::m_pInput{};
size_t CInputDirectInput::m_useCount{};

//-----------------
// ������
//-----------------
HRESULT CInputDirectInput::Register(HINSTANCE hInstanse, HWND hWnd)
{
	//�C���v�b�g�I�u�W�F�N�g�̍쐬
	if (m_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstanse,
			DIRECTINPUT_VERSION, IID_IDirectInput8,
			(void**)&m_pInput,
			nullptr
		)))
		{
			return E_FAIL;
		}
	}

	m_useCount++; // DirectInput�Q�Ɛ�
}

//-----------------
// �j��
//-----------------
void CInputDirectInput::Unregister(void)
{
	m_useCount--; // DirectInput�Q�Ɛ�

	if (m_useCount <= 0)
	{// �Q�Ɛ���0�ȉ�
		//�C���v�b�g�I�u�W�F�N�g�̔j��
		if (m_pInput != nullptr)
		{
			m_pInput->Release();
			m_pInput = nullptr;
		}
	}
}

//-----------------
// ��
//-----------------
HRESULT CInputDirectInput::SetEnum(void)
{
	// DirectInput�f�o�C�X���
	m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);
}

//--------------------------
//
// DirectInput�L�[�{�[�h
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputDirectInputKeyboard::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse, hWnd);

	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr)))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//�t�H�[�}�b�g�쐬
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	// �o�b�t�@�����O��L���ɂ���
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//�A�N�Z�X�����̎擾
	if (FAILED(m_pDevice->Acquire()))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputDirectInputKeyboard::Uninit(void)
{
	// �f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// DirectInput��̂ւ̓o�^����
	CInputDirectInput::Unregister();
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputKeyboard::GetState(span<BYTE> state) const
{
	for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
	{
		m_KeyStateOld[nCntkey] = m_KeyState[nCntkey]; //�O��̃L�[����ۑ�
	}

	BYTE KeyState[MAX_KEY];//�ꎞ���͊i�[
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		for (size_t nCntkey = 0; nCntkey < MAX_KEY; nCntkey++)
		{
			m_KeyState[nCntkey] = KeyState[nCntkey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = (m_KeyState[nCntkey] & 0x80) != 0;    // �������Ă��邩�ǂ���
			bool old = (m_KeyStateOld[nCntkey] & 0x80) != 0; // �O�񉟂��Ă������ǂ���

			m_bKeyPress[nCntkey] = now;           // ��
			m_bKeyTrigger[nCntkey] = now && !old; // ����
			m_bKeyRelease[nCntkey] = (!now && old && m_KeyPressTime[nCntkey] >= CInput::RELEASE_TIME) ? true : false; // �����[�X

			if (m_bKeyTrigger[nCntkey] || m_bKeyRelease[nCntkey])
			{// �L�[�̕ω�
				// �^�C���̏�����
				m_KeyPressTime[nCntkey] = 0.0f;
				m_KeyRepeatTime[nCntkey] = 0.0f;
			}
			else if (m_bKeyPress[nCntkey])
			{// �����Ă���
				// �^�C���̑���
				m_KeyPressTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
				if (m_KeyPressTime[nCntkey] >= CInput::REPEAT_START_TIME)
				{// ���s�[�g��
					m_KeyRepeatTime[nCntkey] += CMain::GetDeltaTimeGameSpeed();
					if (m_KeyRepeatTime[nCntkey] >= CInput::REPEAT_INTERVAL_TIME)
					{// ���s�[�g�Ԋu
						m_KeyRepeatTime[nCntkey] = 0.0f;
						m_bKeyRepeat[nCntkey]= now ? true : false;
					}
				}
			}
		}
	}
	else
	{
		m_pDevice->Acquire();//�A�N�Z�X�����̎擾
	}
}

//--------------------------
//
// DirectInput�}�E�X
//
//--------------------------

//--------------------
//����������
//--------------------
HRESULT CInputDirectInputMouse::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse, hWnd);

	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	//�t�H�[�}�b�g�쐬
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	// �o�b�t�@�����O��L���ɂ���
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// DirectInput��̂ւ̓o�^����
		CInputDirectInput::Unregister();
		return E_FAIL;
	}

	m_pDevice->Acquire();//�A�N�Z�X�����̎擾

	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputDirectInputMouse::Uninit(void)
{
	// �f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// �C���v�b�g�̔j��
	CInputDirectInput::Unregister();
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputMouse::GetState(span<bool> state) const
{
	m_MouseStateOld = m_MouseState; //�O��̃}�E�X����ۑ�

	DIMOUSESTATE2 MouseState;//�}�E�X���//�ꎞ���͊i�[
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &MouseState)))
	{
		m_MouseState = MouseState;//�ꎞ�I�ȏ��𐳋K���ɓn��

		for (size_t nCntButton = 0; nCntButton < MAX; nCntButton++)
		{
			bool now = (m_MouseState.rgbButtons[nCntButton] & 0x80) != 0;    // �������Ă��邩�ǂ���
			bool old = (m_MouseStateOld.rgbButtons[nCntButton] & 0x80) != 0; // �O�񉟂��Ă������ǂ���

			m_bPress[nCntButton] = now;           // ��
			m_bTrigger[nCntButton] = now && !old; // ����
			m_bRelease[nCntButton] = (!now && old && m_PressTime[nCntButton] >= RELEASE_TIME) ? true : false;

			if (m_bTrigger[nCntButton] || m_bRelease[nCntButton])
			{
				m_PressTime[nCntButton] = 0;
			}
			else if (m_bPress[nCntButton])
			{
				m_PressTime[nCntButton]++;
			}

			m_bRepeat[nCntButton] = (now && m_PressTime[nCntButton] >= REPEAT_TIME) ? true : false;

			m_Move[0] = (float)m_MouseState.lX;
			m_Move[1] = (float)m_MouseState.lY;
			m_Move[0] /= MOUSE_INA;
			m_Move[1] /= MOUSE_INA;

			m_WheelMove = (float)m_MouseState.lZ;
			m_WheelMove /= MOUSE_WHEEL_INA;
		}
	}
	else
	{
		m_pDevice->Acquire();//�A�N�Z�X�����̎擾
	}
}

//-----------------------------
//
//dinput�p�b�h
//
//-----------------------------

//--------------------
//����������
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd)
{
	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse, hWnd);

	//�t�H�[�}�b�g�쐬
	m_pDevice->SetDataFormat(&c_dfDIJoystick);

	m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	m_pDevice->Acquire();//�A�N�Z�X�����̎擾

	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//�G�t�F�N�g�f�o�C�X�̔j��
	Vibrate(0, 0);

	// �f�o�C�X�̔j��
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// �C���v�b�g�̔j��
	CInputDirectInput::Unregister();
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputController::GetState(span<bool> state) const
{
	DIJOYSTATE  djoykeyState[CONTROLLER_MAX];//�ꎞ���͊i�[
	int ControllerNum = 0;

	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (g_DevdJoypad[i] != nullptr)
		{
			if (SUCCEEDED(g_DevdJoypad[i]->GetDeviceState(sizeof(DIJOYSTATE), &djoykeyState[i])))
			{
				g_djoykeyState[i] = djoykeyState[i];//�ꎞ�I�ȏ��𐳋K���ɓn��
			}
			else
			{
				ControllerNum++;
			}
		}
		else
		{
			ControllerNum++;
		}
	}

	if (g_DcontrollerNum!=(CONTROLLER_MAX-ControllerNum))
	{
		SetUnCon(true);
		InputReSet();
		// DirectInput�f�o�C�X���
		g_pinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, nullptr, DIEDFL_ATTACHEDONLY);
	}
}

//------------------------------
//�L�[����������
//------------------------------
bool CInputDirectInputController::IsRepeat(int key)
{
	static bool bOldkey[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { false };
	static int KeyPutTime[CONTROLLER_MAX][NUM_DIJOYKEY_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (bOldkey[i][key] == true && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				return true;
			}
			else if (bOldkey[i][key] == false && ((g_djoykeyState[i].rgbButtons[key] & 0x80) ? true : false) == true)
			{
				KeyPutTime[i][key]++;

				if (KeyPutTime[i][key] >= REPEAT_TIME)
				{
					KeyPutTime[i][key] = 0;
					bOldkey[i][key] = true;
				}
			}
			else
			{
				bOldkey[i][key] = false;
				KeyPutTime[i][key] = 0;
			}
		}

		return false;
	}

	if (bOldkey[Controller][key] == true && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		return true;
	}
	else if (bOldkey[Controller][key] == false && ((g_djoykeyState[Controller].rgbButtons[key] & 0x80) ? true : false) == true)
	{
		KeyPutTime[Controller][key]++;

		if (KeyPutTime[Controller][key] >= REPEAT_TIME)
		{
			KeyPutTime[Controller][key] = 0;
			bOldkey[Controller][key] = true;
		}
		return false;
	}
	else
	{
		bOldkey[Controller][key] = false;
		KeyPutTime[Controller][key] = 0;
		return false;
	}
}

//-----------------
//�X�e�B�b�N����
//-----------------
float CInputDirectInputController::GetLeftStickAngle(void)
{
	static float fStick[2] = { 0.0f };

	switch (Stick)
	{
	case STICK_LEFT:
		fStick[0] = (float)g_djoykeyState[Controller].lX;
		fStick[1] = (float)g_djoykeyState[Controller].lY;
		break;
	case STICK_RIGHT:
		fStick[0] = (float)g_djoykeyState[Controller].lZ;
		fStick[1] = (float)g_djoykeyState[Controller].lRz;
		break;
	}

	fStick[0] /= STICK_NUM;
	fStick[1] /= STICK_NUM;

	fStick[0] += DSTICK_NUM;
	fStick[1] += DSTICK_NUM;

	return &fStick[0];
}

//----------------------
//�X���C�_�[��
//----------------------
float CInputDirectInputController::GetSlider(int nSlider)
{
	float fSlider = 0.0f;

	fSlider = (float)g_djoykeyState[Controller].rglSlider[nSlider];

	fSlider /= STICK_NUM;

	fSlider += DSTICK_NUM;

	return fSlider;
}

//------------------------
//�n�b�g�X�C�b�`
//------------------------
bool CInputDirectInputController::IsPressUp(int nPov)
{
	float fPov = 0.0f;

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (fPov==fData)
			{
				return true;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	return (fPov == fData) ? true : false;
}

//-------------------------
//
//-------------------------
bool GetdJoyPovTrigger(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		return false;
	}
	else
	{
		return false;
	}
}

//-----------------------
//
//-----------------------
bool GetdJoyPovRelease(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return true;
				}
				else
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					return false;
				}
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				return false;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;
				return false;
			}
		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == false)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= RELEASE_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return true;
		}
		else
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			return false;
		}
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		return false;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;
		return false;
	}
	else
	{
		return false;
	}
}

//---------------------
//
//---------------------
bool GetdJoyPovRepeat(float fData, int nPov, CONTROLLER Controller)
{
	float fPov = 0.0f;
	static bool bOldPov[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { false };
	static int PovPutTime[CONTROLLER_MAX][POV_MAX][POV_SYS_MAX] = { 0 };

	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			fPov = (float)g_djoykeyState[i].rgdwPOV[nPov];

			if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
			{
				return true;
			}
			else if (bOldPov[i][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
			{
				PovPutTime[i][nPov][(int)(fData / POV_NUM)]++;

				if (PovPutTime[i][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
				{
					PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
					bOldPov[i][nPov][(int)(fData / POV_NUM)] = true;
				}
				return false;
			}
			else
			{
				bOldPov[i][nPov][(int)(fData / POV_NUM)] = false;
				PovPutTime[i][nPov][(int)(fData / POV_NUM)] = 0;
			}


		}

		return false;
	}

	fPov = (float)g_djoykeyState[Controller].rgdwPOV[nPov];

	if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == true && ((fPov == fData) ? true : false) == true)
	{
		return true;
	}
	else if (bOldPov[Controller][nPov][(int)(fData / POV_NUM)] == false && ((fPov == fData) ? true : false) == true)
	{
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)]++;

		if (PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] >= REPEAT_TIME)
		{
			PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
			bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = true;
		}
		return false;
	}
	else
	{
		bOldPov[Controller][nPov][(int)(fData / POV_NUM)] = false;
		PovPutTime[Controller][nPov][(int)(fData / POV_NUM)] = 0;
		return false;
	}
}

//-------------
//�U��
//-------------
void CInputDirectInputController::Vibrate(WORD wLeftMotorSpeed, WORD wRightMotorSpeed)
{
	if (m_effect != nullptr)
	{
		m_effect->Stop();
		m_effect->Release();
		m_effect = nullptr;
	}

	// �t�H�[�X�t�B�[�h�o�b�N�̃G�t�F�N�g���쐬����
	DIEFFECT effect;
	ZeroMemory(&effect, sizeof(DIEFFECT));
	effect.dwSize = sizeof(DIEFFECT);
	effect.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	effect.dwDuration = INFINITE;  // �����ɑ����U��
	effect.dwGain = DI_FFNOMINALMAX;
	effect.dwTriggerButton = DIEB_NOTRIGGER;

	// ���ƕ����̐ݒ�
	DWORD rgdwAxes[1] = { DIJOFS_X };  // X���̐U��
	LONG rglDirection[1] = { 0 };      // �����͐������i0�j
	effect.cAxes = 1;                  // 1��
	effect.rgdwAxes = rgdwAxes;        // �g�p���鎲
	effect.rglDirection = rglDirection;// �U���̕���

	DICONSTANTFORCE cf;
	cf.lMagnitude = lMotorPower;  // �U���̋��� (0 - DI_FFNOMINALMAX)

	effect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
	effect.lpvTypeSpecificParams = &cf;

	if (SUCCEEDED(m_pDevice->CreateEffect(GUID_ConstantForce, &effect, &m_effect, nullptr)))
	{// �쐬�ł���
		if (FAILED(m_effect->Start(1, 0)))
		{// ���s
			m_effect->Release();
			m_effect = nullptr;
		}
	}
}

//--------------------------------
//�ڑ��m�F
//--------------------------------

//-----------
//Xinput
//-----------
bool IsXInputControllerConnected(CONTROLLER Controller)
{
	XINPUT_STATE joykeyState[CONTROLLER_MAX];

	if (Controller==CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (XInputGetState(i, &joykeyState[i]) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(Controller, &joykeyState[Controller]) == ERROR_SUCCESS;
}

//------------
//dinput
//------------
bool IsDirectInputControllerConnected(CONTROLLER Controller)
{
	if (Controller == CONTROLLER_MAX)
	{
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			if (g_DevdJoypad[i] != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	return g_DevdJoypad[Controller] != nullptr;
}

//------------------
//�R���g���[���[��
//------------------
char *ControllerName(CONTROLLER Controller)
{
	return &g_ConName[Controller][0];
}

//------------------
//�R���g���[���[��
//------------------
int ControllerNum(CONTYPE Contype)
{
	if (Contype==CONTYPE_X)
	{
		return g_XcontrollerNum;
	}
	else
	{
		return g_DcontrollerNum;
	}
}

//---------------------------
//�f�o�C�X���擾
//---------------------------
BOOL CALLBACK CInputDirectInput::EnumDevicesCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	static int nControllerNum = 0;
	static int XcontrollerNum = 0;

	if (g_bUnCon)
	{
		nControllerNum = 0;
		XcontrollerNum = 0;
		g_DcontrollerNum = 0;
		g_XcontrollerNum = 0;
		g_bX = false;
		g_bUnCon = false;
		//�G�t�F�N�g�f�o�C�X�̔j��
		VibratedControllerStop(CONTROLLER_MAX);
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			//�L�[�{�[�h�f�o�C�X�̔j��
			if (g_DevdJoypad[i] != nullptr)
			{
				g_DevdJoypad[i]->Unacquire();
				g_DevdJoypad[i]->Release();
				g_DevdJoypad[i] = nullptr;
			}
		}
		for (int i = 0; i < CONTROLLER_MAX; i++)
		{
			g_ConName[i][0] = '\0';
		}
	}

	if ((nControllerNum+XcontrollerNum)<=CONTROLLER_MAX)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		g_pDiInstance[nControllerNum] = pdidInstance;

		if (XInputGetState(XcontrollerNum, &state) == ERROR_SUCCESS)
		{
			if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[XcontrollerNum], nullptr)))
			{//�C���v�b�g�f�o�C�X�̍쐬
				HINSTANCE hInstanse;
				HWND hWnd;

				hInstanse = GethInstanse();
				hWnd = GethWnd();

				//�t�H�[�}�b�g�쐬
				g_DevdJoypad[nControllerNum]->SetDataFormat(&c_dfDIJoystick);

				g_DevdJoypad[nControllerNum]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

				g_DevdJoypad[nControllerNum]->Acquire();//�A�N�Z�X�����̎擾

				strcpy(&g_ConName[nControllerNum][0], pdidInstance->tszProductName);
				XcontrollerNum++;
			}
		}
		else if (SUCCEEDED(g_pinput->CreateDevice(pdidInstance->guidInstance, &g_DevdJoypad[nControllerNum + XNum()], nullptr)))
		{//�C���v�b�g�f�o�C�X�̍쐬
			HINSTANCE hInstanse;
			HWND hWnd;

			hInstanse = GethInstanse();
			hWnd = GethWnd();

			//�t�H�[�}�b�g�쐬
			g_DevdJoypad[nControllerNum + XNum()]->SetDataFormat(&c_dfDIJoystick);

			g_DevdJoypad[nControllerNum + XNum()]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE));

			g_DevdJoypad[nControllerNum + XNum()]->Acquire();//�A�N�Z�X�����̎擾

			strcpy(&g_ConName[nControllerNum + XNum()][0], pdidInstance->tszProductName);
			nControllerNum++;
		}
	}
	g_DcontrollerNum = nControllerNum+XcontrollerNum;

	// �񋓂𑱍s
	return DIENUM_CONTINUE;
}

//----------
//
//----------
int XNum(void)
{
	int XNum = 0;
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			XNum++;
		}
	}
	return XNum;
}

LPDIRECTINPUT8 GetInput(void)
{
	return g_pinput;
}

void SetUnCon(bool bUnCon)
{
	g_bUnCon = bUnCon;
}

//-----------------------
//�R���g���[���[�Ǘ�������
//-----------------------
void InputReSet(void)
{
	g_bUnCon = false;
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		g_ConName[i][0] = '\0';
	}
	g_XcontrollerNum = 0;
	g_DcontrollerNum = 0;
    g_bX = false;
	//�G�t�F�N�g�f�o�C�X�̔j��
	VibratedControllerStop(CONTROLLER_MAX);
	for (int i = 0; i < CONTROLLER_MAX; i++)
	{
		//�L�[�{�[�h�f�o�C�X�̔j��
		if (g_DevdJoypad[i] != nullptr)
		{
			g_DevdJoypad[i]->Unacquire();
			g_DevdJoypad[i]->Release();
			g_DevdJoypad[i] = nullptr;
		}
	}
}

//------------------------------------------------
//�R���g���[���[���擾
//------------------------------------------------
const DIDEVICEINSTANCE* GetDiInstance(CONTROLLER Controller)
{
	return g_pDiInstance[Controller];
}

//----------------------------------------------
//Xinput�R���g���[���[���擾
//----------------------------------------------
XINPUT_STATE GetXstate(CONTROLLER Controller)
{
	return g_joykeyState[Controller];
}

//----------------------------------------------
//dinput�R���g���[���[���擾
//----------------------------------------------
DIJOYSTATE Getdstate(CONTROLLER Controller)
{
	return g_djoykeyState[Controller];
}

//#endif