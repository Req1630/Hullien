#include "Main.h"
#include <Windows.h>

#include "..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\Utility\Log\MyLog.h"

// ImGui�Ŏg�p.
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM );

int WINAPI WinMain( 
	HINSTANCE hInstance, 
	HINSTANCE hInstancePrev, 
	LPSTR pCmdLine, 
	int nCmdShow )
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG.

	CLog::OpenLogTextFile();
	std::unique_ptr<CMain> pMain = std::make_unique<CMain>();

	if( pMain == nullptr ) return 0;

	// �E�B���h�E�̍쐬.
	if( FAILED( pMain->InitWindow( hInstance ) )) return 0;
	// ���C������������.
	if( FAILED( pMain->Init() )) return 0;

	// ���C�����[�v.
	pMain->Loop();

	// ���C���������.
	pMain->Release();

	CLog::CloseLogTextFile();

	return 0;
}

LRESULT CALLBACK WndProc(
	HWND hWnd, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam )
{
	if( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;
	IMGUI_API
	switch( uMsg ){
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
		//���ʂ̏���.
		switch( static_cast<char>(wParam) ){
		case VK_ESCAPE:	//ESC��.
#ifdef _DEBUG
			if( MessageBox( nullptr, "�Q�[�����I�����܂����H", "�x��", MB_YESNO ) == IDYES ){ 
				PostQuitMessage(0); 
			}
			break;
#endif	// #ifdef _DEBUG.
		break;
		}
		break;
	case WM_CLOSE:
		// �E�B���h�E��j������.
		DestroyWindow( hWnd );

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}