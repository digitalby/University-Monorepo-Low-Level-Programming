// SP_LR10.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR10.h"

#define MAX_LOADSTRING 100
#define LR10_RB_RED 278
#define LR10_RB_GREEN 279
#define LR10_RB_BLUE 280
#define LR10_RB_RHOMBUS 281
#define LR10_RB_SQUARE 282
#define LR10_RB_CIRCLE 283
#define LR10_RB_STAR 284
#define LR10_CHECKBOX 285

#define LR10_WPARAM_RED 0
#define LR10_WPARAM_GREEN 1
#define LR10_WPARAM_BLUE 2

#define LR10_LPARAM_RHOMBUS 0
#define LR10_LPARAM_SQUARE 1
#define LR10_LPARAM_CIRCLE 2
#define LR10_LPARAM_STAR 3

#define LR10_WANT_TO_DRAW WM_USER + 1
#define LR10_DRAW_NOW WM_USER + 2

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClass2(HINSTANCE hInstance);

BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProc2(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HWND radiobuttonRed;
HWND radiobuttonGreen;
HWND radiobuttonBlue;

HWND radiobuttonRhombus;
HWND radiobuttonSquare;
HWND radiobuttonCircle;
HWND radiobuttonStar;

HWND checkboxDraw;

HWND mainHWnd;
HWND canvasWindow;

WORD xPos, yPos;

int currentColor = -1, currentShape = -1;


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
    LoadStringW(hInstance, IDC_SPLR10, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClass2(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR10));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR10));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR10);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClass2(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc2;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR10));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPLR10);
    wcex.lpszClassName = L"Child";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
      CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);
   mainHWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   HWND hWndCanvas = CreateWindowW(L"Child", L"Canvas", WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWndCanvas)
       return FALSE;

   canvasWindow = hWndCanvas;

   checkboxDraw = CreateWindow(L"BUTTON",
       L"Draw",
       WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
       20,
       110,
       100,
       20,
       hWnd,
       (HMENU)LR10_CHECKBOX,
       hInst,
       NULL);

   radiobuttonRed = CreateWindow(L"BUTTON",
       L"Red",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
       20, 20, 100, 20, hWnd, (HMENU)LR10_RB_RED, hInst, NULL);
   radiobuttonGreen = CreateWindow(L"BUTTON",
       L"Green",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
       20, 50, 100, 20, hWnd, (HMENU)LR10_RB_GREEN, hInst, NULL);
   radiobuttonBlue = CreateWindow(L"BUTTON",
       L"Blue",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
       20, 80, 100, 20, hWnd, (HMENU)LR10_RB_BLUE, hInst, NULL);
   radiobuttonRhombus = CreateWindow(L"BUTTON",
       L"Rhombus",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP,
       140, 20, 100, 20, hWnd, (HMENU)LR10_RB_RHOMBUS, hInst, NULL);
   radiobuttonSquare = CreateWindow(L"BUTTON",
       L"Square",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
       140, 50, 100, 20, hWnd, (HMENU)LR10_RB_SQUARE, hInst, NULL);
   radiobuttonCircle = CreateWindow(L"BUTTON",
       L"Circle",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
       140, 80, 100, 20, hWnd, (HMENU)LR10_RB_CIRCLE, hInst, NULL);
   radiobuttonStar = CreateWindow(L"BUTTON",
       L"Star",
       WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
       140, 110, 100, 20, hWnd, (HMENU)LR10_RB_STAR, hInst, NULL);

   if (!radiobuttonRed || !radiobuttonGreen || !radiobuttonBlue) {
       MessageBox(NULL, L"Error: color radiobuttons", L"Error", MB_OK);
   }

   if (!radiobuttonRhombus || !radiobuttonSquare || !radiobuttonCircle || !radiobuttonStar) {
       MessageBox(NULL, L"Error: shape radiobuttons", L"Error", MB_OK);
   }

   if (!checkboxDraw) {
       MessageBox(NULL, L"Error: checkbox", L"Error", MB_OK);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   ShowWindow(hWndCanvas, nCmdShow);
   UpdateWindow(hWndCanvas);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case LR10_WANT_TO_DRAW: {
        auto isChecked = SendMessage(checkboxDraw, BM_GETCHECK, NULL, NULL);
        if (isChecked != BST_CHECKED) break;
        auto isCheckedRed = SendMessage(radiobuttonRed, BM_GETCHECK, NULL, NULL);
        auto isCheckedGreen = SendMessage(radiobuttonGreen, BM_GETCHECK, NULL, NULL);
        auto isCheckedBlue = SendMessage(radiobuttonBlue, BM_GETCHECK, NULL, NULL);
        auto isCheckedRhombus = SendMessage(radiobuttonRhombus, BM_GETCHECK, NULL, NULL);
        auto isCheckedSquare = SendMessage(radiobuttonSquare, BM_GETCHECK, NULL, NULL);
        auto isCheckedCircle = SendMessage(radiobuttonCircle, BM_GETCHECK, NULL, NULL);
        auto isCheckedStar = SendMessage(radiobuttonStar, BM_GETCHECK, NULL, NULL);
        WPARAM wParam;
        LPARAM lParam;
        if (isCheckedRed == BST_CHECKED)
            wParam = (WPARAM)LR10_WPARAM_RED;
        if (isCheckedGreen == BST_CHECKED)
            wParam = (WPARAM)LR10_WPARAM_GREEN;
        if (isCheckedBlue == BST_CHECKED)
            wParam = (WPARAM)LR10_WPARAM_BLUE;
        if (isCheckedRhombus == BST_CHECKED)
            lParam = (LPARAM)LR10_LPARAM_RHOMBUS;
        if (isCheckedSquare == BST_CHECKED)
            lParam = (LPARAM)LR10_LPARAM_SQUARE;
        if (isCheckedCircle == BST_CHECKED)
            lParam = (LPARAM)LR10_LPARAM_CIRCLE;
        if (isCheckedStar == BST_CHECKED)
            lParam = (LPARAM)LR10_LPARAM_STAR;
        SendMessageW(canvasWindow, LR10_DRAW_NOW, wParam, lParam);
        break;
    }
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

LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);
        SendMessage(mainHWnd, LR10_WANT_TO_DRAW, NULL, NULL);
        break;
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
        SelectObject(hdc, (HBRUSH)(COLOR_WINDOW + 1));

        Rectangle(hdc, 0, 0, 2000, 2000);
        switch (currentColor) {
        case LR10_WPARAM_RED:
            SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
            break;
        case LR10_WPARAM_GREEN:
            SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));

            break;
        case LR10_WPARAM_BLUE:
            //SelectObject(GetDC(hWnd), CreateSolidBrush(RGB(0, 0, 255)));
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));

            break;
        default:
            break;
        }
        switch (currentShape) {
        case LR10_LPARAM_RHOMBUS:
            POINT rhombusPoints[5];
            rhombusPoints[0].x = xPos - 16;
            rhombusPoints[0].y = yPos;
            rhombusPoints[1].x = xPos;
            rhombusPoints[1].y = yPos + 16;
            rhombusPoints[2].x = xPos + 16;
            rhombusPoints[2].y = yPos;
            rhombusPoints[3].x = xPos;
            rhombusPoints[3].y = yPos - 16;

            Polygon(hdc, rhombusPoints, 4);

            break;
        case LR10_LPARAM_SQUARE:
            Rectangle(hdc, xPos - 16, yPos - 16, xPos + 16, yPos + 16);
            break;
        case LR10_LPARAM_CIRCLE:
            Ellipse(hdc, xPos - 16, yPos - 16, xPos + 16, yPos + 16);
            break;
        case LR10_LPARAM_STAR:
            POINT starPoints[3];
            starPoints[0].x = xPos - 16;
            starPoints[0].y = yPos - 8;
            starPoints[1].x = xPos;
            starPoints[1].y = yPos + 16;
            starPoints[2].x = xPos + 16;
            starPoints[2].y = yPos - 8;
            Polygon(hdc, starPoints, 3);
            starPoints[0].x = xPos - 16;
            starPoints[0].y = yPos + 8;
            starPoints[1].x = xPos;
            starPoints[1].y = yPos - 16;
            starPoints[2].x = xPos + 16;
            starPoints[2].y = yPos + 8;
            Polygon(hdc, starPoints, 3);
            break;
        default:
            break;
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case LR10_DRAW_NOW: {
        switch (LOWORD(wParam)) {
        case LR10_WPARAM_RED:
        case LR10_WPARAM_GREEN:
        case LR10_WPARAM_BLUE: 
            currentColor = LOWORD(wParam);
            break;
        default:
            MessageBox(hWnd, L"Wparam error for custom message", L"Error", MB_OK);
            break;
        }
        switch (LOWORD(lParam)) {
        case LR10_LPARAM_RHOMBUS:
        case LR10_LPARAM_SQUARE:
        case LR10_LPARAM_CIRCLE:
        case LR10_LPARAM_STAR:
            currentShape = LOWORD(lParam);
            break;
        default:
            MessageBox(canvasWindow, L"Lparam error for custom message", L"Error", MB_OK);
            break;
        }
        InvalidateRect(canvasWindow, NULL, TRUE);
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
