// SP_LR13.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SP_LR13.h"
#include <psapi.h>

#define MAX_LOADSTRING 100

#define LR13_MAINLISTBOX 333
#define LR13_SECONDARYLISTBOX 334

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void UpdateProcesses();

HWND mainHWnd, mainListBox, secondaryListBox;
DWORD allProcesses[1024];

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
    LoadStringW(hInstance, IDC_SPLR13, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLR13));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLR13));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLR13);
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

   mainHWnd = hWnd;

   mainListBox = CreateWindow(
       L"LISTBOX",
       L"Main",
       WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
       10,
       10,
       500,
       650,
       hWnd,
       (HMENU)LR13_MAINLISTBOX,
       hInstance,
       NULL
   );
   secondaryListBox = CreateWindow(
       L"LISTBOX",
       L"Secondary",
       WS_CHILD | WS_VISIBLE | WS_BORDER |WS_VSCROLL,
       600,
       10,
       400,
       650,
       hWnd,
       (HMENU)LR13_SECONDARYLISTBOX,
       hInstance,
       NULL
   );

   UpdateProcesses();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void UpdateProcesses() {
    int lastSelection = SendMessage(mainListBox, LB_GETCURSEL, 0, 0);
    TCHAR stringOfProcess[1024];
    if (lastSelection != LB_ERR) {
        SendMessage(mainListBox, LB_GETTEXT, lastSelection, (LPARAM)stringOfProcess);
    }
    DWORD numberOfProcessesFound, processesFoundSize;
    if (!EnumProcesses(allProcesses, sizeof(allProcesses), &processesFoundSize))
        return;
    numberOfProcessesFound = processesFoundSize / sizeof(DWORD);

    SendMessage(mainListBox, LB_RESETCONTENT, 0, 0);
    SendMessage(secondaryListBox, LB_RESETCONTENT, 0, 0);

    for (int i = 0; i < numberOfProcessesFound; i++) {
        if (allProcesses[i] != 0) {
            TCHAR nameOfProcess[MAX_PATH] = L"<unknown>";
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, allProcesses[i]);
            if (hProcess != NULL) {
                HMODULE hModule;
                DWORD cbNeeded; //bytes needed to display string

                if (EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded)) {
                    GetModuleBaseName(hProcess, hModule, nameOfProcess, sizeof(nameOfProcess) / sizeof(TCHAR));
                }

                TCHAR finalName[1024];
                _stprintf(finalName, L"%s (PID: %u)", nameOfProcess, allProcesses[i]);

                SendMessage(mainListBox, LB_ADDSTRING, 0, (LPARAM)finalName);

                CloseHandle(hProcess);
            }
        }
    }

    if (lastSelection != LB_ERR) {
        SendMessage(mainListBox, LB_FINDSTRINGEXACT, -1, (LPARAM)stringOfProcess);
    }
}

void DisplayProcessModules(int index) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, allProcesses[index]);
    if (hProcess == NULL)
        return;
    SendMessage(secondaryListBox, LB_RESETCONTENT, 0, 0);
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];

            if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                sizeof(szModName) / sizeof(TCHAR)))
            {
                TCHAR finalName[1024];

                _stprintf(finalName, L"\t%s (0x%08X)\n", szModName, hMods[i]);
                SendMessage(secondaryListBox, LB_ADDSTRING, 0, (LPARAM)finalName);
            }
        }
    }

    CloseHandle(hProcess);
}

void SetProcessPriority(int choice) {
    int selectedItem = SendMessage(mainListBox, LB_GETCURSEL, 0, 0);
    if (selectedItem == LB_ERR) return;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, allProcesses[selectedItem]);
    if (hProcess == NULL)
        return;
    switch (choice) {
    case 1: //Realtime
        SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
        break;
    case 2: {//High
        SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
        break;
    }
    case 3: //Above normal
        SetPriorityClass(hProcess, ABOVE_NORMAL_PRIORITY_CLASS);
        break;
    case 4: //Normal
        SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);
        break;
    case 5: //Below normal
        SetPriorityClass(hProcess, BELOW_NORMAL_PRIORITY_CLASS);
        break; 
    case 6: //Low
        SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS);
        break;
    default:
        return;
    }

    CloseHandle(hProcess);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CONTEXTMENU: {
        if ((HWND)wParam == mainListBox) {
            //MessageBox(mainHWnd, L"Test", L"Test", MB_OK);
            HMENU hPopUpMenu = CreatePopupMenu();
            int selectedItem = SendMessage(mainListBox, LB_GETCURSEL, 0, 0);
            if (selectedItem == LB_ERR)
                break;
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, allProcesses[selectedItem]);
            if (hProcess == NULL)
                break;
            auto currentPriority = GetPriorityClass(hProcess);
            InsertMenu(hPopUpMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED , 1, L"Realtime");
            InsertMenu(hPopUpMenu, 1, MF_BYCOMMAND | MF_STRING | MF_ENABLED, 2, L"High");
            InsertMenu(hPopUpMenu, 2, MF_BYCOMMAND | MF_STRING | MF_ENABLED , 3, L"Above Normal");
            InsertMenu(hPopUpMenu, 3, MF_BYCOMMAND | MF_STRING | MF_ENABLED, 4, L"Normal");
            InsertMenu(hPopUpMenu, 4, MF_BYCOMMAND | MF_STRING | MF_ENABLED, 5, L"Below Normal");
            InsertMenu(hPopUpMenu, 5, MF_BYCOMMAND | MF_STRING | MF_ENABLED, 6, L"Idle");

            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            int result = TrackPopupMenu(hPopUpMenu,
                TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION,
                x, y, 0, mainHWnd, NULL);
            if (result != 0)
                SetProcessPriority(result);
            CloseHandle(hProcess);
        }
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
            case LR13_MAINLISTBOX:
                switch (HIWORD(wParam)) {
                case LBN_SELCHANGE: {
                    int count = SendMessage(mainListBox, LB_GETCOUNT, 0, 0);
                    int selectedItem = SendMessage(mainListBox, LB_GETCURSEL, 0, 0);
                    if (selectedItem != LB_ERR) {
                        DisplayProcessModules(selectedItem);
                    }
                    break;
                }
                }
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
