// SP_LR7.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR7.h"

#define MAX_LOADSTRING 100

#define LR7_ADD 106
#define LR7_CLEAR 116
#define LR7_DELETE 1006
#define LR7_TORIGHT 1016

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND listBoxLeft;
HWND listBoxRight;
HWND edit;
HWND buttonAdd;
HWND buttonClear;
HWND buttonToRight;
HWND buttonDelete;

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
    LoadStringW(hInstance, IDC_SPLR7, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR7));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR7));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR7);
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

   listBoxLeft = CreateWindow(
       L"LISTBOX",
       L"Left",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       25,
       25,
       140,
       200,
       hWnd,
       NULL,
       hInstance,
       NULL
       );
   listBoxRight = CreateWindow(
       L"LISTBOX",
       L"Right",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       175,
       25,
       140,
       200,
       hWnd,
       NULL,
       hInstance,
       NULL
   );
   edit = CreateWindow(
       L"EDIT",
       L"",
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       25,
       240,
       150,
       20,
       hWnd,
       NULL,
       hInstance,
       NULL
   );
   buttonAdd = CreateWindow(
       L"BUTTON",
       L"Add",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       25,
       260,
       50,
       20,
       hWnd,
       (HMENU)LR7_ADD,
       hInstance,
       NULL
   );
   buttonClear = CreateWindow(
       L"BUTTON",
       L"Clear",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       80,
       260,
       50,
       20,
       hWnd,
       (HMENU)LR7_CLEAR,
       hInstance,
       NULL
   );
   buttonDelete = CreateWindow(
       L"BUTTON",
       L"Delete",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       140,
       260,
       50,
       20,
       hWnd,
       (HMENU)LR7_DELETE,
       hInstance,
       NULL
   );
   buttonToRight = CreateWindow(
       L"BUTTON",
       L"ToRight",
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       200,
       260,
       50,
       20,
       hWnd,
       (HMENU)LR7_TORIGHT,
       hInstance,
       NULL
   );

   if (!listBoxLeft || !listBoxRight || !edit ||
       !buttonAdd || !buttonClear || !buttonDelete || !buttonToRight) {
       return FALSE;
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
            case LR7_ADD: {
                TCHAR buffer[1024];
                GetWindowText(edit, buffer, 1024);
                SetWindowText(edit, L"");
                if (SendMessage(listBoxLeft, LB_FINDSTRINGEXACT, 1, (LPARAM)buffer) == CB_ERR)
                    SendMessage(listBoxLeft, LB_ADDSTRING, 0, (LPARAM)buffer);
                break;
            }
            case LR7_CLEAR:
                SendMessage(listBoxLeft, LB_RESETCONTENT, 0, 0);
                SendMessage(listBoxRight, LB_RESETCONTENT, 0, 0);
                break;
            case LR7_DELETE: {
                int indexLeft = SendMessage(listBoxLeft, LB_GETCURSEL, 0, 0);
                if (indexLeft != LB_ERR)
                    SendMessage(listBoxLeft, LB_DELETESTRING, indexLeft, 0);
                int indexRight = SendMessage(listBoxRight, LB_GETCURSEL, 0, 0);
                if (indexRight != LB_ERR)
                    SendMessage(listBoxRight, LB_DELETESTRING, indexRight, 0);
                break;
            }
            case LR7_TORIGHT: {
                int indexLeft = SendMessage(listBoxLeft, LB_GETCURSEL, 0, 0);
                if (indexLeft == LB_ERR)
                    break;
                TCHAR buffer[1024];
                SendMessage(listBoxLeft, LB_GETTEXT, indexLeft, (LPARAM)buffer);
                if (SendMessage(listBoxRight, LB_FINDSTRINGEXACT, 1, (LPARAM)buffer) == CB_ERR)
                    SendMessage(listBoxRight, LB_ADDSTRING, 0, (LPARAM)buffer);
                break;
            }
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
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
