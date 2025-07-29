//------------------------------------------
//
// エントリーポイント [main.cpp]
// Author: fuma sato
//
//------------------------------------------
#include <memory>
#include "application.h"
#include "common.h"

//------------------------
//メイン関数
//------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    try
    {
        std::unique_ptr<Application, common::UninitDeleter<Application>> pApplication(new Application);
        return pApplication->Run(hInstance, nCmdShow);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), "Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    catch (...)
    {
        MessageBoxA(NULL, "An unknown fatal error occurred.", "Fatal Error", MB_OK | MB_ICONERROR);
        return -1;
    }
}
