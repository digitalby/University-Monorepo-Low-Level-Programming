// SP_LR12.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR12.h"
#include <strsafe.h> <STRSAFE.H>

#define MAX_LOADSTRING 100
#define LR12_MAINEDIT 333
#define LR12_EVENT1 L"LR12_EVENT1"
#define LR12_EVENT2 L"LR12_EVENT2"
#define LR12_EVENT3 L"LR12_EVENT3"

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

HWND mainEdit;
int dataThread1, dataThread2, dataThread3;
HANDLE hThread1, hThread2, hThread3, hThreads[3];
HANDLE hEvent1, hEvent2, hEvent3;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLR12, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR12));

    MSG msg;

    hEvent1 = CreateEventW(NULL, true, false, LR12_EVENT1);
    hEvent2 = CreateEventW(NULL, true, false, LR12_EVENT2);
    hEvent3 = CreateEventW(NULL, true, false, LR12_EVENT3);

    if (!hEvent1 || !hEvent2 || !hEvent3)
        return FALSE;

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR12));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR12);
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
      CW_USEDEFAULT, 0, 150, 150, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   mainEdit = CreateWindow(
       L"EDIT",
       L"",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       10,
       10,
       100,
       20,
       hWnd,
       (HMENU)LR12_MAINEDIT,
       hInst,
       NULL
   );

   if (!mainEdit)
       return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

DWORD WINAPI Thread1(LPVOID lpParam) {
    SetWindowText(mainEdit, L"First");

    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, false, LR12_EVENT1);
    if (!hEvent) { return -1; }
    Sleep(1000);
    SetEvent(hEvent);
    ResetEvent(hEvent);
    CloseHandle(hEvent);

    return 0;
}

DWORD WINAPI Thread2(LPVOID lpParam) {
    WaitForSingleObject(hEvent1, INFINITE);
    SetWindowText(mainEdit, L"Second");
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, false, LR12_EVENT2);
    if (!hEvent) { return -1; }
    Sleep(1000);
    SetEvent(hEvent);
    ResetEvent(hEvent);
    CloseHandle(hEvent);

    return 0;
}

DWORD WINAPI Thread3(LPVOID lpParam) {
    WaitForSingleObject(hEvent1, INFINITE);
    WaitForSingleObject(hEvent2, INFINITE);
    SetWindowText(mainEdit, L"Third");
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, false, LR12_EVENT3);
    if (!hEvent) { return -1; }

    ResetEvent(hEvent);
    CloseHandle(hEvent);

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
