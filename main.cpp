//------------------------------------------
//
//���C���̏���[main.cpp]
//Author: fuma sato
//
//------------------------------------------
#include "main.h"
#include "resource.h"
#include "manager.h"
#include "gui.h"
#include "debug.h"
#include "input.h"

#include <ShellScalingApi.h>  // SetProcessDpiAwareness�p
#include <Shlwapi.h> // �p�X����p (PathRelativePathTo�Ȃ�)

#pragma comment(lib, "winmm.lib")

// C++�̕W�����C�u����
#pragma comment(lib, "Shcore.lib")  // SetProcessDpiAwareness�p
#pragma comment(lib, "Shlwapi.lib") // Shlwapi���C�u�����������N

// CMain�N���X�̐ÓI�����o�ϐ��̒�`
HWND CMain::m_hWnd = nullptr;        // �E�C���h�E�n���h��
bool CMain::m_bFullScreen = false;   // �t���X�N���[���t���O
float CMain::m_elapsedTime = 0.0f;   // ���s����
float CMain::m_deltaTime = 0.0f;     // �t���[������
bool CMain::m_bStop = false;         // �X�g�b�v�t���O
float CMain::m_gameSpeed = 1.0f;     // �Q�[���X�s�[�h

//------------------------
//���C���֐�
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//�E�C���h�E�ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),                                  // �������T�C�Y
		CS_OWNDC,                                            // �X�^�C��
		CMain::WindowProc,                                   // �v���V�[�W��
		0,                                                   // �N���X�̒ǉ�������
		0,                                                   // �E�B���h�E�̒ǉ�������
		hInstance,                                           // �C���X�^���X�n���h��
		LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MYAPP)),      // �^�X�N�o�[�A�C�R��
		LoadCursor(hInstance,MAKEINTRESOURCE(IDC_MYCURSOR)), // �}�E�X�J�[�\��
		nullptr,                                             // �w�i�F
		nullptr,                                             // ���j���[�o�[												
		CMain::CLASS_NAME,                                   // �N���X�̖��O
		LoadIcon(hInstance,MAKEINTRESOURCE(IDI_SMALL))       // �A�C�R��
	};

	MSG msg = { 0 };//���b�Z�[�W
	RECT rect = { 0,0,(LONG)CMain::SCREEN_WIDTH,(LONG)CMain::SCREEN_HEIGHT};//�E�C���h�E�T�C�Y�̐ݒ�

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈�̃T�C�Y����
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�̃X�^�C����ݒ�
	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	//�E�C���h�E�𐶐�
	HWND hWnd = nullptr;
	hWnd = CreateWindowEx
	(
		0,                        //�X�^�C��
		CMain::CLASS_NAME,        //�E�C���h�E�N���X�̖��O
		CMain::WINDOW_NAME,       //�E�C���h�E�̖��O
		style,                    //�X�^�C��
		CW_USEDEFAULT,            //������W
		CW_USEDEFAULT,            //�E�����W
		(rect.right - rect.left), //��
		(rect.bottom - rect.top), //����
		nullptr,                  //�e
		nullptr,                  //���j���[ID
		hInstance,                //�C���X�^���X�n���h��
		nullptr                   //�쐬�f�[�^
	);

	//�\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CMain::SethWnd(hWnd);

	// ����������
	DragAcceptFiles(hWnd, TRUE);

	// Input�o�^
	CMain::RegisterAllInputDevices();

	// �}�l�[�W���[�̐���
	CManager* pManager = nullptr;
	if (pManager == nullptr)
	{
		pManager = new CManager;
	}
	
	// �}�l�[�W���[�̏�����
	if (pManager != nullptr)
	{
		if (FAILED(pManager->Init(hInstance, hWnd, (CMain::ISBORDERLESS || !CMain::IsFullScreen()))))
		{
			return E_FAIL;
		}
	}

	timeBeginPeriod(1);//����\�̐ݒ�

	float Frame = (1.0f / (float)CMain::FPS);                    // �t���[��(�b)
	steady_clock::time_point startTime = steady_clock::now();    // �J�n���� (�����P��)
	steady_clock::time_point exceLastTime = steady_clock::now(); // �O�񎞍� (�����P��)
	steady_clock::time_point currntTime = steady_clock::now();   // ���ݎ��� (�����P��)

	//���b�Z�[�W���[�v
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)//�I�����b�Z�[�W
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		currntTime = steady_clock::now();                                     // �������擾 (�����P��)

		float elapsedTime = duration<float>(currntTime - startTime).count();  // �o�ߎ��� (�b)
		float deltaTime = duration<float>(currntTime - exceLastTime).count(); // �t���[���Ԋu (�b)

		CMain::SetElapsedTime(elapsedTime);                                   // �o�^
		CMain::SetDeltaTime(deltaTime);                                       //

		if (deltaTime >= Frame)//�ݒ肵���Ԋu���󂢂���
		{// �X�V
			exceLastTime = currntTime; //����̍X�V�������L�^

			// �f�o�b�N�\��
			CDebugProc::Print("FPS:%f", 1.0f / deltaTime);
			CDebugProc::Print("�I��:[ESC]");
			CDebugProc::Print("�t���X�N���[��:[F11]");
			CDebugProc::Print(CDebugProc::MODE::Window, "�o�ߎ���:%f", elapsedTime);
			CDebugProc::Print(CDebugProc::MODE::Window, "�t���[������:%f", deltaTime);

			if (!CMain::IsStop())
			{// �X�V��~��ԂłȂ�
				pManager->Update(); //�X�V����
			}
			if (FAILED(pManager->Draw())) { return E_FAIL; }   //�`�揈��
		}
	}

	SAFE_UNINIT(pManager); // �}�l�[�W���[�̔j��

	//�E�C���h�E�N���X�̓o�^����
	UnregisterClass(CMain::CLASS_NAME, wcex.hInstance);

	timeEndPeriod(1);//����\��߂�

	//�I��
	return (int)msg.wParam;
}

//--------------------------
//�E�C���h�E�v���V�[�W��
//--------------------------
LRESULT CALLBACK CMain::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID = 0;//�Ԃ�l�̊i�[

	switch (uMsg)
	{
	case WM_DESTROY://�j��
		if (m_bFullScreen && !ISBORDERLESS)
		{// �t���X�N���[�����[�h�̎�
			ChangeDisplaySettings(nullptr, 0); // �E�C���h�E���[�h��߂�
		}
		//WM_QUIT���b�Z�[�W
		PostQuitMessage(0);
		break;
	case WM_INPUT:
	{
		// �T�C�Y�̎��O�擾
		UINT rawInputSize{};
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &rawInputSize, sizeof(RAWINPUTHEADER));

		// �������̊m��
		vector<BYTE> data(rawInputSize);

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, data.data(), &rawInputSize, sizeof(RAWINPUTHEADER)) == -1)
		{// �f�[�^�擾���s
			CDebugProc::Print(CDebugProc::MODE::System, "Error: GetRawInputData failed!");
			break;
		}

		RAWINPUT* pRawInput = reinterpret_cast<RAWINPUT*>(data.data()); // RAWINPUT�^�ɂ���

		if (pRawInput->header.dwType == RIM_TYPEKEYBOARD)
		{// �L�[�{�[�h�C�x���g
			CDebugProc::Print("--- Keyboard WM_INPUT Received ---");

			USHORT vkey = pRawInput->data.keyboard.VKey;
			USHORT flags = pRawInput->data.keyboard.Flags;

			if (!(flags & RI_KEY_BREAK))
			{// ����
				switch (vkey)
				{// �ǂ̃L�[��
				case VK_ESCAPE: //ESC�L�[
					nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
					if (nID == IDYES)
					{// �͂�
						//�j��(WM_DESTROY)
						DestroyWindow(hWnd);
					}
					break;
				case VK_F11: //F11�L�[
					if (ISBORDERLESS)
					{// �{�[�_�[���X�E�B���h�E���[�h�̎�
						ToggleBorderless();
					}
					else
					{// �t���X�N���[�����[�h�̎�
						ToggleFullScreen();
					}
					break;
				}
			}
		}
		else if (pRawInput->header.dwType == RIM_TYPEMOUSE)
		{// �}�E�X�C�x���g
			CDebugProc::Print(CDebugProc::MODE::System, "--- Mouse WM_INPUT Received ---");
		}
		else if (pRawInput->header.dwType == RIM_TYPEHID)
		{// HID�C�x���g
			CDebugProc::Print(CDebugProc::MODE::System, "--- HID WM_INPUT Received ---");
		}

		CInputRawInput::AnalysisRawData(pRawInput); // �|�C���^��Input���ɔC����
		break;
	}
	case WM_SIZE:
		switch (wParam)
		{
		case SIZE_MINIMIZED:
			m_bStop = true;
			break;
		default:
			if (FAILED(CManager::RenererReset((CMain::ISBORDERLESS || !m_bFullScreen))))
			{
				return E_FAIL; // �����_���[�̃��Z�b�g
			}
			m_bStop = false;
			break;
		}
		break;
	case WM_ENTERSIZEMOVE:
		m_bStop = true;
		break;
	case WM_EXITSIZEMOVE:
		m_bStop = false;
		break;
	case WM_KILLFOCUS:
		m_bStop = true;
		break;
	case WM_SETFOCUS:
		m_bStop = false;
		break;
	case WM_ACTIVATEAPP:
		if (wParam == TRUE)
		{
			m_bStop = false;
		}
		else
		{
			m_bStop = true;
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_MAXIMIZE)//�ő剻�{�^��
		{
			m_bStop = false;
		}
		else if ((wParam & 0xFFF0) == SC_MINIMIZE)//�ŏ����{�^��
		{
			m_bStop = true;
		}
		else if ((wParam & 0xFFF0) == SC_CLOSE)//�ŏ����{�^��
		{
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION);
			if (nID == IDNO)
			{
				return 0;
			}
		}
		break;
	case WM_CLOSE://�E�C���h�E�́~�{�^��
		break;
	case WM_DISPLAYCHANGE://�f�B�X�v���C�̕ύX
		break;
	case WM_DEVICECHANGE:
		// Input�o�^
		CMain::RegisterAllInputDevices();
		break;
	case WM_POWERBROADCAST:
		break;
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		char filePath[MAX_PATH] = { 0 };
		DragQueryFile(hDrop, 0, filePath, MAX_PATH);
		DragFinish(hDrop);
		// �h���b�v���ꂽ�t�@�C���̃p�X���擾
		string_view droppedFilePath = filePath;
		if (!droppedFilePath.empty())
		{
			// �t�@�C���p�X���g�p���鏈���������ɒǉ�
			MessageBox(hWnd, droppedFilePath.data(), "Dropped File", MB_OK);
		}
		break;
	}
	}

	CManager::GetGui().Proc(hWnd, uMsg, wParam, lParam); // Gui

	//�K�v�f�[�^��Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//----------------------------
// �t���X�N���[��
//----------------------------
void CMain::ToggleFullScreen(void)
{
	m_bFullScreen = !m_bFullScreen;
	static RECT windowRect = {};

	// �E�B���h�E�X�^�C��
	static LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	static LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if (m_bFullScreen)
	{
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		GetWindowRect(m_hWnd, &windowRect);

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		DEVMODE dm = {};
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = (DWORD)screenWidth;    // �𑜓x
		dm.dmPelsHeight = (DWORD)screenHeight;
		dm.dmBitsPerPel = 32; // �t���J���[
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		ChangeDisplaySettings(&dm, CDS_FULLSCREEN);

		LONG newStyle = style & ~(WS_OVERLAPPEDWINDOW);
		SetWindowLong(m_hWnd, GWL_STYLE, newStyle);

		SetWindowPos
		(
			m_hWnd,
			HWND_TOP,
			0, 0,
			screenWidth, screenHeight,
			SWP_FRAMECHANGED
		);
	}
	else
	{
		ChangeDisplaySettings(nullptr, 0); // �f�B�X�v���C�ݒ�����ɖ߂�

		SetWindowLong(m_hWnd, GWL_STYLE, style);

		SetWindowPos
		(
			m_hWnd,
			nullptr,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED
		);
	}

	//�\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//----------------------------
// �{�[�_�[���X�E�B���h�E
//----------------------------
void CMain::ToggleBorderless(void)
{
	m_bFullScreen = !m_bFullScreen;
	static RECT windowRect = {};

	// �E�B���h�E�X�^�C��
	static LONG style = GetWindowLong(m_hWnd, GWL_STYLE);
	static LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);

	if (m_bFullScreen)
	{
		// ���݂̃E�B���h�E�ʒu�ƃT�C�Y�ƃX�^�C����ۑ�
		style = GetWindowLong(m_hWnd, GWL_STYLE);
		exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		GetWindowRect(m_hWnd, &windowRect);

		// �t���X�N���[�����[�h�p�̃X�^�C���ɕύX
		// �{�[�_���X�E�B���h�E�������̗p

		LONG newStyle = style & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		LONG newExStyle = exStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

		// �X�^�C����K�p
		SetWindowLong(m_hWnd, GWL_STYLE, newStyle);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, newExStyle);

		// ��ʃT�C�Y���擾
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// �E�B���h�E���X�N���[���T�C�Y�ɍ��킹��
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			0, 0,
			screenWidth, screenHeight,
			SWP_FRAMECHANGED
		);
	}
	else
	{
		// �ʏ�E�B���h�E���[�h�ɖ߂�
		SetWindowLong(m_hWnd, GWL_STYLE, style);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);

		// ���̃E�B���h�E�T�C�Y�ƈʒu�ɖ߂�
		SetWindowPos(
			m_hWnd,
			HWND_TOP,
			windowRect.left, windowRect.top,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			SWP_FRAMECHANGED
		);
	}

	// �\���ƍX�V
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//----------------------------
// �t�@�C���p�X�擾
//----------------------------
string_view CMain::GetFilePath(void)
{
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	char currentDir[MAX_PATH] = "";

	// ���݂̃f�B���N�g�����擾
	GetCurrentDirectory(MAX_PATH, currentDir);
	strcat_s(currentDir, MAX_PATH, "\\data\0");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFilter = "�摜�t�@�C��\0*.jpg;*.jpeg;*.png;*.bmp\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	ofn.lpstrInitialDir = currentDir; // �����f�B���N�g����ݒ�

	if (GetOpenFileName(&ofn))
	{
		return string_view(fileName);
	}

	return "";
}

//----------------------------
// �N���C�A���g�̈�擾
//----------------------------
HRESULT CMain::GetClientRect(RECT* rect)
{
	if (rect == nullptr)
	{
		return E_FAIL;
	}
	// �E�B���h�E�̃N���C�A���g�̈���擾
	if (!::GetClientRect(m_hWnd, rect))
	{
		return E_FAIL;
	}
	return S_OK;
}

//-----------------
// �G���[
//-----------------
void CMain::Error(HRESULT hr, const char func[])
{
	char msg[256];
	snprintf(msg, sizeof(msg), "���s���܂����B\nHRESULT: 0x%08X", hr);
	MessageBox(m_hWnd, msg, func, MB_OK | MB_ICONERROR);
	PostMessage(m_hWnd, WM_KEYDOWN, VK_ESCAPE, 0);
}

//------------------
//�}�E�X����
//------------------
void CMain::ConfineCursorToWindow(void)
{
	RECT rect;
	GetClientRect(&rect);

	rect.left += 10;
	rect.top += 10;
	rect.right -= 10;
	rect.bottom -= 10;

	MapWindowPoints(m_hWnd, NULL, (POINT*)&rect, 2);
	ClipCursor(&rect);
}

//-----------------
//�}�E�X���
//-----------------
void CMain::ReleaseCursor(void)
{
	ClipCursor(nullptr); // ����������
}

//-----------------
// �}�E�X�ؑ�
//-----------------
void CMain::CursorSwitch(bool Switch)
{
	if (Switch == true)
	{
		ReleaseCursor();
		while (ShowCursor(TRUE) < 0);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0);
		ConfineCursorToWindow();
	}
}

//----------------------------------------------------
// �}�E�X�J�[�\���̈ʒu���擾���� (�N���C�A���g���W)
//----------------------------------------------------
POINT CMain::GetCursorPoint(void)
{
	POINT point;
	if (!GetCursorPos(&point)) return POINT(~0); // �J�[�\���̃X�N���[�����W

	// �X�N���[�����W���N���C�A���g���W�ɕϊ�����
	return ScreenToClient(m_hWnd, &point) ? point : POINT(~0);
}

//----------------------------------------------------
// �C���v�b�g�̓o�^ (�X�^���_�[�h)
//----------------------------------------------------
void CMain::RegisterStandardInputDevices(void)
{
	RAWINPUTDEVICE RawDevice[] =
	{
		{ 0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, m_hWnd }, // Standard Keyboard
		{ 0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, m_hWnd }, // Standard Mouse
		{ 0x0C, 0x01, RIDEV_INPUTSINK, m_hWnd },                  // Consumer Control
	};

	if (RegisterRawInputDevices(RawDevice, 3, sizeof(RAWINPUTDEVICE)))
	{
		CDebugProc::Print(CDebugProc::MODE::Static, "Standard input devices registered successfully");
	}
	else
	{
		DWORD error = GetLastError();
		CDebugProc::Print(CDebugProc::MODE::Static, "Standard registration failed: %d", error);
	}
}

//----------------------------------------------------
// �C���v�b�g�̓o�^ (ALL)
//----------------------------------------------------
void CMain::RegisterAllInputDevices(void)
{
	try
	{
		UINT nDevices = 0;
		if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
		{
			CDebugProc::Print(CDebugProc::MODE::Static, "Failed to get raw input device count");
			return;
		}

		if (nDevices == 0)
		{
			CDebugProc::Print(CDebugProc::MODE::Static, "No raw input devices found");
			return;
		}

		std::vector<RAWINPUTDEVICELIST> devices(nDevices);
		if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
		{
			CDebugProc::Print(CDebugProc::MODE::Static, "Failed to get raw input device list");
			return;
		}

		std::vector<RAWINPUTDEVICE> inputDevices;
		std::set<std::pair<USHORT, USHORT>> registeredPairs;

		for (const auto& dev : devices)
		{
			RID_DEVICE_INFO info = {};
			UINT size = sizeof(info);
			info.cbSize = sizeof(info);

			if (GetRawInputDeviceInfo(dev.hDevice, RIDI_DEVICEINFO, &info, &size) > 0)
			{
				RAWINPUTDEVICE rawDevice = {};
				rawDevice.dwFlags = RIDEV_INPUTSINK;
				rawDevice.hwndTarget = m_hWnd;

				if (info.dwType == RIM_TYPEKEYBOARD)
				{
					// �L�[�{�[�h�̏ꍇ�͕W���I��UsagePage/Usage���g�p
					USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
					USHORT usage = 0x06;                 // HID_USAGE_GENERIC_KEYBOARD
					rawDevice.dwFlags |= RIDEV_NOLEGACY | RIDEV_NOHOTKEYS; // NOLEGACY_NOHOTKEYS

					auto pair = std::make_pair(usagePage, usage);
					if (registeredPairs.find(pair) == registeredPairs.end())
					{
						registeredPairs.insert(pair);
						rawDevice.usUsagePage = usagePage;
						rawDevice.usUsage = usage;
						inputDevices.push_back(rawDevice);

						CDebugProc::Print(CDebugProc::MODE::Static, "Adding keyboard: %04x:%04x (Type: %d, SubType: %d, KeyboardMode: %d)",
							usagePage, usage, info.keyboard.dwType, info.keyboard.dwSubType, info.keyboard.dwKeyboardMode);
					}
				}
				else if (info.dwType == RIM_TYPEMOUSE)
				{
					// �}�E�X�̏ꍇ�͕W���I��UsagePage/Usage���g�p
					USHORT usagePage = 0x01;             // HID_USAGE_PAGE_GENERIC
					USHORT usage = 0x02;                 // HID_USAGE_GENERIC_MOUSE

					auto pair = std::make_pair(usagePage, usage);
					if (registeredPairs.find(pair) == registeredPairs.end())
					{
						registeredPairs.insert(pair);
						rawDevice.usUsagePage = usagePage;
						rawDevice.usUsage = usage;
						inputDevices.push_back(rawDevice);

						CDebugProc::Print(CDebugProc::MODE::Static, "Adding mouse: %04x:%04x (ID: %d, Buttons: %d, SampleRate: %d)",
							usagePage, usage, info.mouse.dwId, info.mouse.dwNumberOfButtons, info.mouse.dwSampleRate);
					}
				}
				else if (info.dwType == RIM_TYPEHID)
				{
					// HID�f�o�C�X�̏ꍇ�̂� info.hid ���g�p
					USHORT usagePage = info.hid.usUsagePage;
					USHORT usage = info.hid.usUsage;

					// �L����UsagePage/Usage���`�F�b�N
					if (usagePage != 0)
					{
						auto pair = std::make_pair(usagePage, usage);
						if (registeredPairs.find(pair) == registeredPairs.end())
						{
							registeredPairs.insert(pair);
							rawDevice.usUsagePage = usagePage;
							rawDevice.usUsage = usage;
							inputDevices.push_back(rawDevice);

							CDebugProc::Print(CDebugProc::MODE::Static, "Adding HID device: %04x:%04x (VendorId: %04x, ProductId: %04x)",
								usagePage, usage, info.hid.dwVendorId, info.hid.dwProductId);
						}
					}
				}
			}
		}

		if (!inputDevices.empty())
		{
			if (RegisterRawInputDevices(inputDevices.data(), UINT(inputDevices.size()), sizeof(RAWINPUTDEVICE)))
			{
				DWORD error = GetLastError();
				CDebugProc::Print(CDebugProc::MODE::Static, "Successfully registered %zu devices LastError:%d", inputDevices.size(), error);
			}
			else
			{
				DWORD error = GetLastError();
				CDebugProc::Print(CDebugProc::MODE::Static, "Registration failed: %d", error);
			}
		}
	}
	catch (...)
	{
		CDebugProc::Print(CDebugProc::MODE::Static, "Exception in RegisterAllKeyboardDevices");
	}
}

//----------------------------------------------------
// �f�o�C�X���̏ڍו\���i�f�o�b�O�p�j
//----------------------------------------------------
void CMain::DebugPrintDeviceInfo(void)
{
	try
	{
		UINT nDevices = 0;
		if (GetRawInputDeviceList(nullptr, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
		{
			return;
		}

		std::vector<RAWINPUTDEVICELIST> devices(nDevices);
		if (GetRawInputDeviceList(devices.data(), &nDevices, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
		{
			return;
		}

		CDebugProc::Print(CDebugProc::MODE::Static, "=== Device Information ===");

		for (size_t i = 0; i < devices.size(); ++i)
		{
			const auto& dev = devices[i];
			RID_DEVICE_INFO info = {};
			UINT size = sizeof(info);
			info.cbSize = sizeof(info);

			if (GetRawInputDeviceInfo(dev.hDevice, RIDI_DEVICEINFO, &info, &size) > 0)
			{
				CDebugProc::Print(CDebugProc::MODE::Static, "Device %zu: Type=%d", i, info.dwType);

				switch (info.dwType)
				{
				case RIM_TYPEKEYBOARD:
					CDebugProc::Print(CDebugProc::MODE::Static, "  Keyboard - Type:%d SubType:%d KeyboardMode:%d",
						info.keyboard.dwType, info.keyboard.dwSubType, info.keyboard.dwKeyboardMode);
					break;

				case RIM_TYPEMOUSE:
					CDebugProc::Print(CDebugProc::MODE::Static, "  Mouse - ID:%d Buttons:%d SampleRate:%d HasHorizontalWheel:%s",
						info.mouse.dwId, info.mouse.dwNumberOfButtons, info.mouse.dwSampleRate,
						info.mouse.fHasHorizontalWheel ? "Yes" : "No");
					break;

				case RIM_TYPEHID:
					CDebugProc::Print(CDebugProc::MODE::Static, "  HID - VendorId:%04x ProductId:%04x UsagePage:%04x Usage:%04x",
						info.hid.dwVendorId, info.hid.dwProductId, info.hid.usUsagePage, info.hid.usUsage);
					break;
				}
			}
		}
		CDebugProc::Print(CDebugProc::MODE::Static, "========================");
	}
	catch (...)
	{
		CDebugProc::Print(CDebugProc::MODE::Static, "Exception in DebugPrintDeviceInfo");
	}
}