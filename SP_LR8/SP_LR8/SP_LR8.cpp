// SP_LR8.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR8.h"

#define MAX_LOADSTRING 100

#define LR8_DISPLAY 116
#define LR8_ERASE 1116

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND buttonDisplay;
HWND buttonErase;
HBITMAP displayBmp;
HBITMAP eraseBmp;

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
    LoadStringW(hInstance, IDC_SPLR8, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR8));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR8));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR8);
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

   buttonDisplay = CreateWindow(
       L"BUTTON",
       L"Display",
       WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
       25,
       550,
       50,
       20,
       hWnd,
       (HMENU)LR8_DISPLAY,
       hInstance,
       NULL
   );
   buttonErase = CreateWindow(
       L"BUTTON",
       L"Erase",
       WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
       100,
       550,
       50,
       20,
       hWnd,
       (HMENU)LR8_ERASE,
       hInstance,
       NULL
   );

   if (!buttonDisplay || !buttonErase)
       return FALSE;

   displayBmp = (HBITMAP)LoadImageW(hInstance, L"lr8_disp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
   eraseBmp = (HBITMAP)LoadImageW(hInstance, L"lr8_eras.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

   if (displayBmp == NULL) {
       MessageBox(hWnd, L"BMP display load fail!", L"Load", MB_OK);
   }
   if (eraseBmp == NULL) {
       MessageBox(hWnd, L"BMP erase load fail!", L"Load", MB_OK);
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
    PAINTSTRUCT ps;
    LPDRAWITEMSTRUCT lpdis;
    HDC hdcMem;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case LR8_DISPLAY: {
                lpdis = (LPDRAWITEMSTRUCT)lParam;
                HDC hDC = GetDC(hWnd);
                
                //Body
                SelectObject(hDC, CreateSolidBrush(RGB(255,255,0)));
                Ellipse(hDC, 30, 30, 490, 490);

                //Eyes
                SelectObject(hDC, CreateSolidBrush(RGB(0, 0, 255)));
                Ellipse(hDC, 150, 150, 200, 200);
                Ellipse(hDC, 300, 150, 350, 200);

                //Retinas
                SelectObject(hDC, CreateSolidBrush(RGB(64, 64, 255)));
                Rectangle(hDC, 170, 170, 180, 180);
                Rectangle(hDC, 320, 170, 330, 180);

                //Brows
                SelectObject(hDC, CreateSolidBrush(RGB(0, 0, 0)));
                MoveToEx(hDC, 200, 140, NULL);
                LineTo(hDC, 150, 100);
                MoveToEx(hDC, 300, 140, NULL);
                LineTo(hDC, 350, 100);

                //Smile
                POINT points[3];
                SelectObject(hDC, CreateSolidBrush(RGB(0, 0, 0)));
                MoveToEx(hDC, 100, 250, NULL);
                //LineTo(hDC, 400, 400);
                points[0].x = 100;
                points[0].y = 250;
                points[1].x = 250;
                points[1].y = 280;
                points[2].x = 400;
                points[2].y = 250;
                PolyBezierTo(hDC, points, 3);

                MoveToEx(hDC, 100, 250, NULL);
                points[0].x = 100;
                points[0].y = 250;
                points[1].x = 250;
                points[1].y = 350;
                points[2].x = 400;
                points[2].y = 250;
                PolyBezierTo(hDC, points, 3);

                //Beard
                MoveToEx(hDC, 100, 400, NULL);
                POINT polyPoints[8];
                polyPoints[0].x = 100;
                polyPoints[0].y = 400;
                polyPoints[1].x = 150;
                polyPoints[1].y = 480;
                polyPoints[2].x = 200;
                polyPoints[2].y = 450;
                polyPoints[3].x = 250;
                polyPoints[3].y = 480;
                polyPoints[4].x = 300;
                polyPoints[4].y = 450;
                polyPoints[5].x = 350;
                polyPoints[5].y = 480;
                polyPoints[6].x = 400;
                polyPoints[6].y = 400;
                polyPoints[7].x = 100;
                polyPoints[7].y = 400;
                Polygon(hDC, polyPoints, 8);

                ReleaseDC(hWnd, hDC);
                break;
            }
            case LR8_ERASE: {
                HDC hDC = GetDC(hWnd);
                SelectObject(hDC, (HBRUSH)(COLOR_WINDOW + 1));
                Rectangle(hDC, 20, 20, 500, 500);

                ReleaseDC(hWnd, hDC);
            }
                break;
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
        hdcMem = CreateCompatibleDC(0);
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC hDC = GetDC(hWnd);
        MoveToEx(hDC, 20, 20, NULL);
        LineTo(hDC, 500, 20);
        LineTo(hDC, 500, 500);
        LineTo(hDC, 20, 500);
        LineTo(hDC, 20, 20);
        ReleaseDC(hWnd, hDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DRAWITEM:
        lpdis = (LPDRAWITEMSTRUCT)lParam;
        hdcMem = CreateCompatibleDC(lpdis->hDC);
        switch (LOWORD(wParam)) {
        case LR8_DISPLAY:
            SelectObject(hdcMem, displayBmp);
            BitBlt(lpdis->hDC,
                lpdis->rcItem.left,
                lpdis->rcItem.top,
                lpdis->rcItem.right - lpdis->rcItem.left,
                lpdis->rcItem.bottom - lpdis->rcItem.top,
                hdcMem, 0, 0, SRCCOPY);
            break;
        case LR8_ERASE:
            SelectObject(hdcMem, eraseBmp);
            BitBlt(lpdis->hDC, 0, 0, 50, 20, hdcMem, 0, 0, SRCCOPY);
            break;
        default:
            break;
        }
        DeleteDC(hdcMem);
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
