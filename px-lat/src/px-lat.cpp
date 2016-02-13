// name: px-lat.cpp
// px-lat.cpp : Defines the entry point for the application.
// desc: entry point for windows

#include <windowsx.h>
#include "px-lat.h"

#include <px/shell/wingl.h>
#include <px/core/engine.h>
#include <px/shell/key_bingings.hpp>
#include <px/key.h>

#include <memory>

using px::key;

#define MAX_LOADSTRING 100

// Global Variables:
HWND hWnd;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

std::unique_ptr<px::core::engine> engine; // everything (else)
std::unique_ptr<px::shell::key_bindings<WPARAM, px::key>> bindings;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PXLAT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PXLAT));
	try
	{
		auto wgl = std::make_unique<px::shell::wingl>(hWnd);
		engine = std::make_unique<px::core::engine>(wgl.get());
		bindings = std::make_unique<px::shell::key_bindings<WPARAM, px::key>>();

		bindings->bind('W', VK_UP, VK_NUMPAD8, key::move_north);
		bindings->bind('A', VK_LEFT, VK_NUMPAD4, key::move_west);
		bindings->bind('S', VK_DOWN, VK_NUMPAD2, key::move_south);
		bindings->bind('D', VK_RIGHT, VK_NUMPAD6, key::move_east);
		bindings->bind(VK_END, VK_NUMPAD1, key::move_southwest);
		bindings->bind(VK_NEXT, VK_NUMPAD3, key::move_southeast);
		bindings->bind(VK_HOME, VK_NUMPAD7, key::move_northwest);
		bindings->bind(VK_PRIOR, VK_NUMPAD9, key::move_northeast);
		bindings->bind(VK_SPACE, VK_NUMPAD5, key::move_none);

		bindings->bind('1', key::action1);
		bindings->bind('2', key::action2);
		bindings->bind('3', key::action3);
		bindings->bind('4', key::action4);
		bindings->bind('5', key::action5);
		bindings->bind('6', key::action6);
		bindings->bind('7', key::action7);
		bindings->bind('8', key::action8);
		bindings->bind('9', key::action9);
		bindings->bind('0', key::action0);
		bindings->bind('E', key::action_use);

		bindings->bind(VK_F5, key::quick_save);
		bindings->bind(VK_F9, key::quick_load);

		bindings->bind(VK_RETURN, key::command_ok);
		bindings->bind(VK_ESCAPE, key::command_cancel);

		for (bool run = true; run; run &= engine->running())
		{
			engine->frame();

			// dispatch windows messages
			while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != 0)
			{
				run &= (GetMessage(&msg, NULL, 0, 0) == TRUE);
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
	}
	catch (std::exception &exc)
	{
		wchar_t message[1028];
		MultiByteToWideChar(CP_ACP, 0, exc.what(), -1, message, 1024);
		MessageBox(NULL, message, NULL, NULL);
	}

	return (int) msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PXLAT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//   PURPOSE: Saves instance handle and creates main window
//   COMMENTS:
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (engine && bindings)
		{
			engine->press(bindings->select(wParam, key::not_valid));
		}
		break;
	case WM_MOUSEMOVE:
		if (engine)
		{
			engine->hover({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		}
		break;
	case WM_LBUTTONDOWN:
		if (engine)
		{
			engine->click({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, 1);
		}
		break;
	case WM_RBUTTONDOWN:
		if (engine)
		{
			engine->click({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, 2);
		}
		break;
	case WM_MOUSEWHEEL:
		if (engine)
		{
			engine->scroll(GET_WHEEL_DELTA_WPARAM(wParam));
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}