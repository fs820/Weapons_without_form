//------------------------------------------
//
// �f�o�b�O���� [debug.cpp]
// Author: fuma sato
//
//------------------------------------------
#include "debug.h"
#include "manager.h"

//------------------------
// 
// �f�o�b�O�N���X
// 
//------------------------

// �ÓI�����o�ϐ�
LPD3DXFONT CDebugProc::m_pFont{};                                // �t�H���g�|�C���^
string CDebugProc::m_sDebug[MODE_MAX]{};                         // ������
D3DXCOLOR CDebugProc::m_color{ D3DXCOLOR(1.0f,1.0f,1.0f,1.0f) }; // �F
UINT CDebugProc::m_flag{ DT_LEFT | DT_TOP | DT_WORDBREAK };      // �\�����@
bool CDebugProc::m_bDraw{ true };                                // �������邩
CDebugProc::MODE CDebugProc::m_drawMode{};                       // �\�����郂�[�h

//---------------------
// ������
//---------------------
HRESULT CDebugProc::Init(void)
{
	// �t�H���g�̐���
	return D3DXCreateFont
	(
		CManager::GetRenderer().GetDevice(),
		32, 0,                        // ����32 ���͍����ɍ��킹��
		FW_BOLD,                      // ����
		1,                            // �~�b�v�}�b�v�� 1�ł���
		FALSE,                        // �Α̕���
		SHIFTJIS_CHARSET,             // ���{���
		OUT_TT_PRECIS,                // TrueType�t�H���g�D��
		CLEARTYPE_QUALITY,            // �A���`�G�C���A�X�L��
		DEFAULT_PITCH | FF_DONTCARE,  // �ʏ�
		"Yu Gothic",                  // ����
		&m_pFont
	);
}

//---------------------
// �I��
//---------------------
void CDebugProc::Uninit(void)
{
	// �t�H���g�j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

//---------------------
// �`��
//---------------------
void CDebugProc::Draw(void)
{
	if (m_bDraw)
	{// �\�����
		D3DXVECTOR2 screenSize{};
		CManager::GetRenderer().GetDxScreenSize(&screenSize);
		RECT rect{ 0,0,LONG(screenSize.x),LONG(screenSize.y) };

		m_pFont->DrawText(nullptr, m_sDebug[m_drawMode].c_str(), -1, &rect, m_flag, D3DCOLOR_COLORVALUE(m_color.r, m_color.g, m_color.b, m_color.a));
	}

	for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
	{
		m_sDebug[cntMode].clear(); // ��������N���A
	}
}

////---------------------
//// ������ǉ�
////---------------------
//void CDebugProc::Print(const char* format, ...)
//{
//	va_list args{}; // �ψ����̃A�h���X�����邽�߂�char�|�C���^
//	va_start(args, format); // �|�C���^�̈ʒu���ψ����̎n�܂��
//
//	while (*format != '\0')
//	{// �����񂪏I���܂Ń��[�v
//		if (*format == '%')
//		{// %��������
//			format++; // ���̎��ɍs��
//			switch (*format)
//			{// �^�̔���
//			case 'd': // int
//				m_sDebug += std::to_string_view(va_arg(args, int)); // args�̌����_�̈ʒu�̈�����int�Ƃ��ĂƂ�
//				break;
//			case 'f': // double
//				m_sDebug += std::to_string_view(va_arg(args, double)); // args�̌����_�̈ʒu�̈�����double�Ƃ��ĂƂ�
//				break;
//			case 's': // string_view
//				m_sDebug += va_arg(args, char*); // args�̌����_�̈ʒu�̈����𕶎���Ƃ��ĂƂ�
//				break;
//			}
//		}
//		else
//		{// ���ʂ̕���
//			m_sDebug += *format; // �ǉ�
//		}
//		format++; // ���̕�����
//	}
//
//	va_end(args); // �|�C���^���
//}

//---------------------
// ������ǉ�
//---------------------
void CDebugProc::Print(const char* format, ...)
{
	// �G���[����
	if (format == nullptr)
	{// ������format
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC�ȊO�̕�����ɓK������
			m_sDebug[cntMode] += "������format\n";
		}
		return;
	}

	va_list args{};         // �ψ����̃A�h���X�����邽�߂�char�|�C���^
	va_start(args, format); // �|�C���^�̈ʒu���ψ����̎n�܂��

	// ���O�ɒ������擾����
	va_list checkArgs{};                                       // �����擾�p�̃R�s�[
	va_copy(checkArgs, args);                                  // �R�s�[
	int formatSize = vsnprintf(nullptr, 0, format, checkArgs); // �t�H�[�}�b�g�̒������擾
	va_end(checkArgs);                                         // �|�C���^�J��
	
	// �G���[����
	if (formatSize < 0)
	{// �����ȃT�C�Y
		va_end(args);
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC�ȊO�̕�����ɓK������
			m_sDebug[cntMode] += "������format\n";
		}
		return;
	}
	else if (formatSize == 0)
	{// format����
		va_end(args);
		for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
		{// STATIC�ȊO�̕�����ɓK������
			m_sDebug[cntMode] += "\n";
		}
		return;
	}

	for (size_t cntMode = WINDOW; cntMode < MODE_MAX; cntMode++)
	{// STATIC�ȊO�̕�����ɓK������
		// ������ɒǉ��X�y�[�X��p�ӂ���
		size_t oldSize = m_sDebug[cntMode].size();
		m_sDebug[cntMode].resize(oldSize + formatSize + 1);

		// ������ɒǉ�
		vsnprintf(&m_sDebug[cntMode][oldSize], formatSize + 1, format, args); // �������ǉ�
		m_sDebug[cntMode][oldSize + formatSize] = '\n';                       // null�����s�ɏ㏑��
	}
	va_end(args);                                                        // �|�C���^���
}

//---------------------
// ������ǉ�
//---------------------
void CDebugProc::Print(const MODE mode, const char* format, ...)
{
	// �G���[����
	if (format == nullptr)
	{// ������format
		m_sDebug[mode] += "������format\n";
		return;
	}

	va_list args{};         // �ψ����̃A�h���X�����邽�߂�char�|�C���^
	va_start(args, format); // �|�C���^�̈ʒu���ψ����̎n�܂��

	// ���O�ɒ������擾����
	va_list checkArgs{};                                       // �����擾�p�̃R�s�[
	va_copy(checkArgs, args);                                  // �R�s�[
	int formatSize = vsnprintf(nullptr, 0, format, checkArgs); // �t�H�[�}�b�g�̒������擾
	va_end(checkArgs);                                         // �|�C���^�J��

	// �G���[����
	if (formatSize < 0)
	{// �����ȃT�C�Y
		va_end(args);
		m_sDebug[mode] += "������format\n";
		return;
	}
	else if (formatSize == 0)
	{// format����
		va_end(args);
		m_sDebug[mode] += "\n";
		return;
	}

	// ������ɒǉ��X�y�[�X��p�ӂ���
	size_t oldSize = m_sDebug[mode].size();
	m_sDebug[mode].resize(oldSize + formatSize + 1);

	// ������ɒǉ�
	vsnprintf(&m_sDebug[mode][oldSize], formatSize + 1, format, args); // �������ǉ�
	m_sDebug[mode][oldSize + formatSize] = '\n';                       // null�����s�ɏ㏑��
	va_end(args);                                                      // �|�C���^���
}