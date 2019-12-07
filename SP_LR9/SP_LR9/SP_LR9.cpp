// SP_LR9.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR9.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int timerID;
HBITMAP frames[4];
BOOL isPresent = false;
BOOL isInMotion = false;
int currentFrame = 0;
int millisecondsOfAnimation = 0;
WORD xPos, yPos;
int currentX, currentY, targetX, targetY;

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
    LoadStringW(hInstance, IDC_SPLR9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR9));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR9));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR9);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   frames[0] = (HBITMAP)LoadImageW(hInstance, L"lr9_0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   frames[1] = (HBITMAP)LoadImageW(hInstance, L"lr9_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   frames[2] = (HBITMAP)LoadImageW(hInstance, L"lr9_2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   frames[3] = (HBITMAP)LoadImageW(hInstance, L"lr9_3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   if (!frames[0] || !frames[1] || !frames[2] || !frames[3]) {
       MessageBox(hWnd, L"BMP load fail!", L"Load", MB_OK);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
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
    case WM_TIMER: {
        if (isInMotion == FALSE) {
            break;
        }
        //MessageBox(hWnd, L"BMP moved!", L"Move", MB_OK);

        millisecondsOfAnimation++;
        HDC hdcMem;
        hdcMem = CreateCompatibleDC(GetDC(hWnd));
        if (currentX < targetX)
            currentX++;
        else if (currentX > targetX)
            currentX--;
        if (currentY < targetY)
            currentY++;
        else if (currentY > targetY)
            currentY--;
        if (currentX == targetX && currentY == targetY) {
            isInMotion = FALSE;
            KillTimer(hWnd, 1);
        }
        SelectObject(GetDC(hWnd), (HBRUSH)(COLOR_WINDOW + 1));
        Rectangle(GetDC(hWnd), 0, 0, 2000, 2000);
        SelectObject(hdcMem, frames[currentFrame]);
        BitBlt(GetDC(hWnd),
            currentX - 16,
            currentY - 16,
            currentX + 16,
            currentY + 16,
            hdcMem, 0, 0, SRCCOPY);

        //Switch frames
        if (millisecondsOfAnimation >= 33) {
            currentFrame++;
            if (currentFrame >= 4) {
                currentFrame = 0;
            }
            millisecondsOfAnimation = 0;
        }

        DeleteDC(hdcMem);
        break;
    }
    case WM_LBUTTONDOWN: {
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
        HDC hdcMem;
        hdcMem = CreateCompatibleDC(GetDC(hWnd));
        if (isPresent == FALSE) {
            SelectObject(hdcMem, frames[0]);
            BitBlt(GetDC(hWnd),
                xPos - 16,
                yPos - 16,
                xPos + 16,
                yPos + 16,
                hdcMem, 0, 0, SRCCOPY);
            currentX = targetX = xPos;
            currentY = targetY = yPos;
            isPresent = TRUE;
        }
        else {
            if (isInMotion == FALSE) {
                targetX = xPos;
                targetY = yPos;
                isInMotion = TRUE;
                timerID = SetTimer(hWnd, 1, 1, NULL);
            }
        }
        DeleteDC(hdcMem);
        break;
    }
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
