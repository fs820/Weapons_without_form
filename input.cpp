//---------------------------------------
//
//���͏���[input.cpp]
//Author fuma sato
//
//---------------------------------------
#include"input.h"

#include <cstddef> // offsetof ���g�����߂ɕK�v

using namespace input; // �C���v�b�g��Ԃ̎g�p

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
	return true;
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool CInputKeyboardManager::IsTrigger(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool CInputKeyboardManager::IsRelease(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//�L�[����������
//------------------------------
bool CInputKeyboardManager::IsRepeat(Index8 idx, BUTTON button)
{
	return true;
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
	return true;
}

//------------------------------
//�L�[���͎��̏���
//------------------------------
bool CInputMouseManager::IsTrigger(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//�L�[�b�������ۂ̏���
//------------------------------
bool CInputMouseManager::IsRelease(Index8 idx, BUTTON button)
{
	return true;
}

//------------------------------
//�L�[����������
//------------------------------
bool CInputMouseManager::IsRepeat(Index8 idx, BUTTON button)
{
	return true;
}

//--------------
//�}�E�X�ړ�
//--------------
Axis CInputMouseManager::GetAxis(Index8 idx)
{
	return Axis();
}

//----------------
//�L�[��������
//----------------
float CInputMouseManager::GetWheel(Index8 idx)
{
	return float{};
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
	XInputEnable(true); // XInput�L����
	for (Index8 cnt = 0; cnt < 4u; cnt++)
	{// XInput�m�F
		if (pController != nullptr) return E_FAIL;  // null
		pController = new CInputXInputController;   // XInput����
		if (pController == nullptr)  return E_FAIL; // �������s

		// ������
		if (SUCCEEDED(pController->Init(hInstanse, hWnd, cnt)))
		{// ����
			m_apController.push_back(pController); // �R���g���[���[�ǉ�
			pController = nullptr;                 // null
			continue;                              // ������
		}
		SAFE_UNINIT(pController); // �̂Ă�
	}

	size_t XInputNum = m_apController.size(); // Xinput�̐�
	if (XInputNum < 1)
	{// Xinput�R���g���[���[���Ȃ�����
		XInputEnable(false); // XInput������
	}

	// Direct

	// ���o�^
	if (FAILED(CInputDirectInput::Register(hInstanse)))return E_FAIL;
	// �f�o�C�X��
	if (FAILED(CInputDirectInput::SetEnum(this)))return E_FAIL;
	// ���o�^����
	CInputDirectInput::Unregister();

	for (Index cnt = XInputNum; cnt < m_apController.size(); cnt++)
	{// DirectInput���m�F
		// ������
		if (FAILED(m_apController[cnt]->Init(hInstanse, hWnd)))
		{// ���s
			SwapRemove(m_apController, cnt); // �폜
		}
	}

	// �����������킹���]
	m_apController.shrink_to_fit();

	return S_OK;
}

//-------------------
// ������
//-------------------
void CInputControllerManager::Uninit(void)
{
	SAFE_UNINIT_ARRAY(m_apController); // �R���g���[���[�����ׂĔj��
	XInputEnable(false); // XInput������
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
	return true;
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsTrigger(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsRelease(Index idx, BUTTON button)
{
	return true;
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsRepeat(Index idx, BUTTON button)
{
	return true;
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
bool CInputControllerManager::IsXInputControllerConnected(Index8 idx)
{
	XINPUT_STATE state{};
	if (idx == INVALID_ID)
	{
		for (Index8 cnt = 0; cnt < 3; ++cnt)
		{
			if (XInputGetState(cnt, &state) == ERROR_SUCCESS)
			{
				return true;
			}
		}

		return false;
	}

	return XInputGetState(idx, &state) == ERROR_SUCCESS;
}

//-------------------
// ������
//-------------------
bool CInputControllerManager::IsDirectInputControllerConnected(Index8 idx)
{
	if (idx == INVALID_ID)
	{
		for (Index8 cnt = 0; cnt < m_apController.size(); ++cnt)
		{
			if (m_apController[cnt] != nullptr)
			{
				return true;
			}
		}

		return false;
	}

	return m_apController[idx] != nullptr;
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
	m_keyOld = m_key; // �O��̃t���[����ۑ�

	array <bool, input::MAX_KEY> key{};
	if (SUCCEEDED(GetKey(key)))
	{
		for (Index8 cntKey = 0; cntKey < key.size(); cntKey++)
		{
			m_key[cntKey] = key[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = m_key[cntKey];    // �������Ă��邩�ǂ���
			bool old = m_keyOld[cntKey]; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // �����[�X

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
						m_bRepeat[cntKey] = now;
					}
				}
			}
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
	m_buttonOld = m_button; // �O��̃t���[����ۑ�

	// �{�^���̏�Ԃ����
	array<bool, Index8(input::MOUSE_BUTTON::Max)> button{};
	if (SUCCEEDED(GetButton(button)))
	{
		for (Index8 cntKey = 0; cntKey < button.size(); cntKey++)
		{
			m_button[cntKey] = button[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = m_button[cntKey];    // �������Ă��邩�ǂ���
			bool old = m_buttonOld[cntKey]; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // �����[�X

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
						m_bRepeat[cntKey] = now;
					}
				}
			}
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
	m_buttonOld = m_button; // �O��̃t���[����ۑ�

	// �{�^���̏�Ԃ����
	array<bool, Index8(input::CONTROLLER_BUTTON::Max)> button{};
	if (SUCCEEDED(GetButton(button)))
	{
		for (Index8 cntKey = 0; cntKey < button.size(); cntKey++)
		{
			m_button[cntKey] = button[cntKey]; //�ꎞ�I�ȏ��𐳋K���ɓn��

			bool now = m_button[cntKey];    // �������Ă��邩�ǂ���
			bool old = m_buttonOld[cntKey]; // �O�񉟂��Ă������ǂ���

			m_bPress[cntKey] = now;           // ��
			m_bTrigger[cntKey] = now && !old; // ����
			m_bRelease[cntKey] = (!now && old && m_PressTime[cntKey] >= RELEASE_TIME); // �����[�X

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
						m_bRepeat[cntKey] = now;
					}
				}
			}
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
	return S_OK;
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
HRESULT CInputRawInputKeyboard::GetKey(span<bool> key) const
{
	return S_OK;
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
HRESULT CInputRawInputMouse::GetButton(span<bool> button) const
{
	return S_OK;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputRawInputMouse::GetMove(input::Axis* pMove) const
{
	return S_OK;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputRawInputMouse::GetWheel(float* pWheel) const
{
	return S_OK;
}

//--------------------------
//
//�p�b�h
//
//--------------------------

//--------------------------------
//����������
//--------------------------------
HRESULT CInputXInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	m_idx = idx; // �C���f�b�N�X��ۑ�

	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//�X�V����
//------------------------
HRESULT CInputXInputController::GetButton(span<bool> button) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		SetTriggerButton(&state);
		for (Index8 cntButton = 0; cntButton < Index8(JOYKEY::Max); cntButton++)
		{
			// �ϊ��\����XInput�ԍ�����R���g���[���[���ʂ̔ԍ����擾����
			Index8 controllerButton = Index8(ControllerMapList[Index8(CONTROLLER_TYPE::XInput)][cntButton]);

			// JOYKEY����XInput�悤��Mask�ɕϊ�
			WORD mask = GetXInputMask(JOYKEY(cntButton));

			// �{�^���������Ă��邩�ǂ������擾����
			button[controllerButton] = (state.Gamepad.wButtons & mask) != 0;
		}
		return S_OK;
	}
	return E_FAIL;
}

//------------------------
//�X�V����
//------------------------
HRESULT CInputXInputController::GetStick(span<input::Axis> axis) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// ���X�e�B�b�N
		axis[Left].x = float(state.Gamepad.sThumbLX);
		axis[Left].y = float(state.Gamepad.sThumbLY);

		// �E�X�e�B�b�N
		axis[Right].x = float(state.Gamepad.sThumbRX);
		axis[Right].y = float(state.Gamepad.sThumbRY);

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
//�X�V����
//------------------------
HRESULT CInputXInputController::GetTrigger(span<float> trigger) const
{
	constexpr float TRIGGER_MAX = float(numeric_limits<BYTE>::max());

	XINPUT_STATE XState;
	if (XInputGetState(m_idx, &XState) == ERROR_SUCCESS)
	{
		// ���X�e�B�b�N
		trigger[Left] = float(XState.Gamepad.bLeftTrigger) / TRIGGER_MAX;

		// �E�X�e�B�b�N
		trigger[Right] = float(XState.Gamepad.bRightTrigger) / TRIGGER_MAX;

		return S_OK;
	}

	return E_FAIL;
}

//------------------------
// �X���C�_�[
//------------------------
HRESULT CInputXInputController::GetSlider(span<float> slider) const
{
	XINPUT_STATE state;
	if (XInputGetState(m_idx, &state) == ERROR_SUCCESS)
	{
		// XInput�̃X���C�_�[�̓X�e�B�b�N��Y�������Ă���
		slider[Left] = float(state.Gamepad.sThumbLY) / float(STICK_NUM);
		slider[Right] = float(state.Gamepad.sThumbRY) / float(STICK_NUM);

		return S_OK;
	}

	return E_FAIL;
}

//----------------
//�U��
//----------------
void CInputXInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
	constexpr WORD VIBRATION_MAX = numeric_limits<WORD>::max();
	leftMotorSpeed = clamp(leftMotorSpeed, 0.0f, 1.0f);
	rightMotorSpeed = clamp(rightMotorSpeed, 0.0f, 1.0f);
	XINPUT_VIBRATION vibration{ WORD(float(VIBRATION_MAX) * leftMotorSpeed) ,WORD(float(VIBRATION_MAX) * rightMotorSpeed) };
	XInputSetState(0, &vibration);
}

//-----------------------------------------------
// �g���K�[�̒l��臒l�𒴂�����{�^���ɓ��͂ɑ���
//-----------------------------------------------
void CInputXInputController::SetTriggerButton(PXINPUT_STATE pState) const
{
	// �g���K�[��臒l���m�F
	if (pState->Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// �����Ă�����
		// wButtons�̃r�b�g�Ɋi�[
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_TRIGGER;
	}
	if (pState->Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{// �����Ă�����
		// wButtons�̃r�b�g�Ɋi�[
		pState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_TRIGGER;
	}
}

//-----------------------------------------------
// �񋓌^��XInput�{���̃}�N���ɒu��������
//-----------------------------------------------
WORD CInputXInputController::GetXInputMask(input::JOYKEY key) const
{
	switch (key)
	{
	case JOYKEY::Up:    return XINPUT_GAMEPAD_DPAD_UP;
	case JOYKEY::Down:  return XINPUT_GAMEPAD_DPAD_DOWN;
	case JOYKEY::Left:  return XINPUT_GAMEPAD_DPAD_LEFT;
	case JOYKEY::Right: return XINPUT_GAMEPAD_DPAD_RIGHT;
	case JOYKEY::Start: return XINPUT_GAMEPAD_START;
	case JOYKEY::Back:  return XINPUT_GAMEPAD_BACK;
	case JOYKEY::L3:    return XINPUT_GAMEPAD_LEFT_THUMB;
	case JOYKEY::R3:    return XINPUT_GAMEPAD_RIGHT_THUMB;
	case JOYKEY::LB:    return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case JOYKEY::RB:    return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case JOYKEY::LT:    return XINPUT_GAMEPAD_LEFT_TRIGGER;
	case JOYKEY::RT:    return XINPUT_GAMEPAD_RIGHT_TRIGGER;
	case JOYKEY::A:     return XINPUT_GAMEPAD_A;
	case JOYKEY::B:     return XINPUT_GAMEPAD_B;
	case JOYKEY::X:     return XINPUT_GAMEPAD_X;
	case JOYKEY::Y:     return XINPUT_GAMEPAD_Y;
	default:            return 0;
	}
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
HRESULT CInputDirectInput::Register(HINSTANCE hInstanse)
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

	// �Q�ƒǉ�
	m_useCount++;

	return S_OK;
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
		SAFE_RELEASE(m_pInput);
	}
}

//-----------------
// ��
//-----------------
HRESULT CInputDirectInput::SetEnum(CInputControllerManager* pControllerManager)
{
	//�C���v�b�g�I�u�W�F�N�g�̍쐬
	if (m_pInput != nullptr)
	{
		// DirectInput�f�o�C�X���
		return m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumDevicesCallback, pControllerManager, DIEDFL_ATTACHEDONLY);
	}
	return E_FAIL;
}

//---------------------------
//�f�o�C�X���擾
//---------------------------
BOOL CALLBACK CInputDirectInput::EnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// CInputControllerManager�𒊏o����
	CInputControllerManager* pControllerManager = static_cast<CInputControllerManager*>(pvRef);

	// DirectInput�R���g���[���[�̐���
	CInputDirectInputController* pdController = new CInputDirectInputController;

	// �f�o�C�X����n��
	pdController->SetInstance(lpddi);

	// ���N���X�|�C���^�ɃA�b�v (���S)
	CInputController* pController = pdController;

	// Manager�ɓn��
	pControllerManager->Register(pController);

	// �񋓂𑱍s
	return DIENUM_CONTINUE;
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
	//�C���v�b�g�f�o�C�X�̍쐬 
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysKeyboard, &m_pDevice, nullptr))) return E_FAIL;

	//�t�H�[�}�b�g�쐬
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// �o�b�t�@�����O��L���ɂ���
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// �f�o�C�X�ݒ�
	if (FAILED(SetProperty()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//�A�N�Z�X�����̎擾
	if (FAILED(m_pDevice->Acquire()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse);

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
HRESULT CInputDirectInputKeyboard::GetKey(span<bool> key) const
{
	if (m_pDevice != nullptr)
	{
		BYTE state[MAX_KEY];//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(state), state)))
		{
			for (Index8 cntKey = 0; cntKey < key.size(); cntKey++)
			{
				key[cntKey] = (state[cntKey] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//------------------------
// �f�o�C�X�ݒ�
//------------------------
HRESULT CInputDirectInputKeyboard::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// �f�o�C�X�S�̂ɐݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16�̃C�x���g���i�[�ł���o�b�t�@���쐬
	dipdw.dwData = 16;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
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
	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(GUID_SysMouse, &m_pDevice, nullptr)))return E_FAIL;

	//�t�H�[�}�b�g�쐬
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// �o�b�t�@�����O��L���ɂ���
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// �f�o�C�X�ݒ�
	if (FAILED(SetProperty()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//�A�N�Z�X�����̎擾
	if (FAILED(m_pDevice->Acquire()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse);

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
HRESULT CInputDirectInputMouse::GetButton(span<bool> button) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//�}�E�X���//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			for (Index8 cntButton = 0; cntButton < button.size(); cntButton++)
			{
				button[cntButton] = (state.rgbButtons[cntButton] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputMouse::GetMove(input::Axis* pMove) const
{
	if (m_pDevice != nullptr)
	{
		DIMOUSESTATE2 state{};//�}�E�X���//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			pMove->x = float(state.lX);
			pMove->y = float(state.lY);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputMouse::GetWheel(float* pWheel) const
{
	if (m_pDevice!=nullptr)
	{
		DIMOUSESTATE2 state{};//�}�E�X���//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &state)))
		{
			*pWheel = float(state.lZ);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//------------------------
// �f�o�C�X�ݒ�
//------------------------
HRESULT CInputDirectInputMouse::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// �f�o�C�X�S�̂ɐݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16�̃C�x���g���i�[�ł���o�b�t�@���쐬
	dipdw.dwData = 16;
	return m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
}

//-----------------------------
//
//dinput�p�b�h
//
//-----------------------------

//--------------------
//����������
//--------------------
HRESULT CInputDirectInputController::Init(HINSTANCE hInstanse, HWND hWnd, Index8 idx)
{
	//�C���v�b�g�f�o�C�X�̍쐬
	if (FAILED(CInputDirectInput::GetDirectInput()->CreateDevice(m_pDiInstance->guidInstance, &m_pDevice, nullptr)))return E_FAIL;

	//�t�H�[�}�b�g�쐬
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// �f�o�C�X�ݒ�
	if (FAILED(SetProperty()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	//�A�N�Z�X�����̎擾
	if (FAILED(m_pDevice->Acquire()))
	{
		// �f�o�C�X�̔j��
		SAFE_RELEASE(m_pDevice);
		return E_FAIL;
	}

	// DirectInput��̂ւ̓o�^
	CInputDirectInput::Register(hInstanse);

	return S_OK;
}

//----------------------
//�I������
//----------------------
void CInputDirectInputController::Uninit(void)
{
	//�G�t�F�N�g�f�o�C�X�̔j��
	Vibrate(0);

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
HRESULT CInputDirectInputController::GetButton(span<bool> button) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			for (Index cntButton = 0; cntButton < button.size(); cntButton++)
			{
				button[cntButton] = (state.rgbButtons[0] & 0x80) != 0;
			}
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputController::GetStick(span<input::Axis> axis) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE  state;// ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			axis[Left].x = float(state.lX) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Left].y = float(state.lY) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Right].x = float(state.lRx) / float(DIRECTINPUT_TRIGGER_MAX);
			axis[Right].y = float(state.lRy) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputController::GetTrigger(span<float> trigger) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			trigger[Left] = float(state.lZ) / float(DIRECTINPUT_TRIGGER_MAX);
			trigger[Left] = float(state.lRz) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------------------
//�X�V����
//-------------------------
HRESULT CInputDirectInputController::GetSlider(span<float> slider) const
{
	if (m_pDevice != nullptr)
	{
		DIJOYSTATE state{};//�ꎞ���͊i�[
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)))
		{
			// �X���C�_�[���͂����
			slider[Left] = float(state.rglSlider[Left]) / float(DIRECTINPUT_TRIGGER_MAX);
			slider[Right] = float(state.rglSlider[Right]) / float(DIRECTINPUT_TRIGGER_MAX);
			return S_OK;
		}
		else
		{
			m_pDevice->Acquire();//�A�N�Z�X�����̎擾
		}
	}
	return E_FAIL;
}

//-------------
//�U��
//-------------
void CInputDirectInputController::Vibrate(float leftMotorSpeed, float rightMotorSpeed)
{
	if (m_effect != nullptr)
	{// �G�t�F�N�g��j��
		m_effect->Stop();
		m_effect->Release();
		m_effect = nullptr;
	}

	if (leftMotorSpeed < 0.000001f) return; // 0�̎��͐������Ȃ�

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
	leftMotorSpeed = clamp(leftMotorSpeed, -1.0f, 1.0f);
	cf.lMagnitude = LONG(leftMotorSpeed * float(DI_FFNOMINALMAX));  // �U���̋��� (0 - DI_FFNOMINALMAX)

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

//------------------------
// �f�o�C�X�ݒ�
//------------------------
HRESULT CInputDirectInputController::SetProperty(void)
{
	if (m_pDevice == nullptr)return E_FAIL;

	// �f�o�C�X�S�̂ɐݒ�
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.diph.dwObj = 0;

	// 16�̃C�x���g���i�[�ł���o�b�t�@���쐬
	dipdw.dwData = 16;
	if (FAILED(m_pDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))return E_FAIL;

	// �f�b�h�]�[����20%�ɐݒ�
	dipdw.dwData = 2000;
	if (FAILED(m_pDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph)))return E_FAIL;

	// ���I�u�W�F�N�g�ݒ�
	return m_pDevice->EnumObjects(EnumAxesCallback, this, DIDFT_AXIS);
}

//------------------------
// �͈͐ݒ�R�[���o�b�N
//------------------------
BOOL CALLBACK CInputDirectInputController::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	CInputDirectInputController* pThis = static_cast<CInputDirectInputController*>(pvRef);

	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.diph.dwObj = lpddoi->dwOfs;

	// dwOfs��DIJOYSTATE2�\���̓��̃I�t�Z�b�g�l�Ȃ̂ŁA
	// C++�W����offsetof���g���Ē��ڔ�r����
	switch (lpddoi->dwOfs)
	{
		// �X�e�B�b�N�n�̎� (�������j���[�g����)
	case offsetof(DIJOYSTATE2, lX):
	case offsetof(DIJOYSTATE2, lY):
	case offsetof(DIJOYSTATE2, lRx):
	case offsetof(DIJOYSTATE2, lRy):
		dipr.lMin = DIRECTINPUT_AXIS_MIN;
		dipr.lMax = DIRECTINPUT_AXIS_MAX;
		break;

		// �g���K�[��X���C�_�[�n�̎� (�ŏ��`�ő�)
	case offsetof(DIJOYSTATE2, lZ):
	case offsetof(DIJOYSTATE2, lRz):
	case offsetof(DIJOYSTATE2, rglSlider[0]): // DIJOFS_SLIDER(0) �͂���ɑ���
	case offsetof(DIJOYSTATE2, rglSlider[1]): // DIJOFS_SLIDER(1) �͂���ɑ���
	default: // �s���Ȏ���������ŏ���
		dipr.lMin = DIRECTINPUT_TRIGGER_MIN;
		dipr.lMax = DIRECTINPUT_TRIGGER_MAX;
		break;
	}

	HRESULT hr = pThis->m_pDevice->SetProperty(DIPROP_RANGE, &dipr.diph);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}