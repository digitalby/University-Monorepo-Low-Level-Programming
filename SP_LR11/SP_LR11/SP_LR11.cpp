// SP_LR11.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR11.h"
#include <time.h>
#include <strsafe.h> <STRSAFE.H>
#define _CRT_RAND_S
#include <stdlib.h>

#define MAX_LOADSTRING 100

#define LR11_EDIT1 333
#define LR11_EDIT2 334
#define LR11_EDIT3 335

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI Thread1(LPVOID lpParam);
DWORD WINAPI Thread2(LPVOID lpParam);
DWORD WINAPI Thread3(LPVOID lpParam);

HWND edit1, edit2, edit3;
int dataThread1, dataThread2, dataThread3;
unsigned int seed = time(NULL);
HANDLE hThread1, hThread2, hThread3, hThreads[3];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    srand(time(NULL));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLR11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hThread1 = CreateThread(NULL, 0, Thread1, &dataThread1, 0, NULL);
    if (!hThread1)
        ExitProcess(dataThread1);
    hThread2 = CreateThread(NULL, 0, Thread2, &dataThread2, 0, NULL);
    if (!hThread2)
        ExitProcess(dataThread2);
    hThread3 = CreateThread(NULL, 0, Thread3, &dataThread3, 0, NULL);
    if (!hThread3)
        ExitProcess(dataThread3);

    hThreads[0] = hThread1;
    hThreads[1] = hThread2;
    hThreads[2] = hThread3;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR11));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 150, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   edit1 = CreateWindow(
       L"EDIT",
       L"",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       10,
       10,
       100,
       20,
       hWnd,
       (HMENU)LR11_EDIT1,
       hInst,
       NULL
   );
   edit2 = CreateWindow(
       L"EDIT",
       L"",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       10,
       50,
       100,
       20,
       hWnd,
       (HMENU)LR11_EDIT2,
       hInst,
       NULL
   );
   edit3 = CreateWindow(
       L"EDIT",
       L"",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       10,
       90,
       100,
       20,
       hWnd,
       (HMENU)LR11_EDIT3,
       hInst,
       NULL
   );

   if (!edit1 || !edit2 || !edit3) {
       return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int rand_r(unsigned* seedp) {
    *seedp = *seedp * 1103515245 + 12345;
    return((unsigned)(*seedp / 65536) % 32768);
}

void DisplayRandomNumber(HWND hWnd) {
    TCHAR buffer[1024];
    //seed = time(NULL);
    int randomNumber = rand_r(&seed);
    StringCchPrintf(buffer, 1024,
        TEXT("%d"),
        randomNumber);
    SetWindowText(hWnd, buffer);
    Sleep(2000);
}

DWORD WINAPI Thread1(LPVOID lpParam) {
    while (1) {
        DisplayRandomNumber(edit1);
    }
    return 0;
}

DWORD WINAPI Thread2(LPVOID lpParam) {
    while (1) {
        DisplayRandomNumber(edit2);
    }
    return 0;
}

DWORD WINAPI Thread3(LPVOID lpParam) {
    while (1) {
        DisplayRandomNumber(edit3);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:

        //WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
        //CloseHandle(hThread1);
        //CloseHandle(hThread2);
        //CloseHandle(hThread3);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
