#include <windows.h> 
#include <string> 
const UINT WM_APP_MY_THREAD_UPDATE = WM_APP + 0;

HINSTANCE hInst;
LPCTSTR szWindowClass = L"lb1";
LPCTSTR szTitle = L"LABOne";
int chisone = 1;
int chisten = 10000;
LARGE_INTEGER Fraq;
HANDLE threadInc, threadDec, threadTemp, timeThread;
LARGE_INTEGER StInc, EnInc, StDec, EnDec, StSub, EnSub;
DWORD NumInc(PVOID x);
DWORD NumDicrease(PVOID x);
DWORD Printdiss(PVOID x);
void ReceiveTime(PVOID x);
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance;
	hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		200,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	DWORD id;
	switch (message)
	{
	case WM_CREATE: {
		QueryPerformanceFrequency(&Fraq);
		threadInc = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NumInc, hWnd, 0, NULL);
		threadDec = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NumDicrease, hWnd, 0, NULL);
		threadTemp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Printdiss, hWnd, 0, NULL);
		timeThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveTime, hWnd, 0, NULL);
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_APP_MY_THREAD_UPDATE: { // При переході чисел в іншу разрядність не залишалось число
		GetClientRect(hWnd, &rect);
		rect.left = 200; // обновлюємий відділок
		rect.top = lParam - 20;

		rect.bottom = lParam;
		InvalidateRect(hWnd, &rect, TRUE);
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
DWORD NumInc(PVOID x) {
	QueryPerformanceCounter(&StInc);
	HWND hWnd = (HWND)x;
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveTime, hWnd, 0, NULL);
	int X = 200;
	int Y = 0;
	while (chisone <= 10000) {
		TextOut(hdc, 0, Y, L"First Thread", std::wcslen(L"First Thread"));
		TextOut(hdc, X, Y, std::to_wstring(chisone).c_str(), std::to_wstring(chisone).size());
		chisone++;
		Sleep(10);
		QueryPerformanceCounter(&EnInc);
	}
	Sleep(10);
	ReleaseDC(hWnd, hdc);
	return 0;
}
DWORD NumDicrease(PVOID x) {
	QueryPerformanceCounter(&StDec);
	HWND hWnd = (HWND)x;
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	int X = 200;
	int Y = 20;
	for (; chisten > 0;) {
		if (chisten == 9999)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)40);
		if (chisten == 999)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)40);
		if (chisten == 99)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)40);
		if (chisten == 9)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)40);

		TextOut(hdc, 0, Y, L"Second Thread", std::wcslen(L"Second Thread"));
		TextOut(hdc, X, Y, std::to_wstring(chisten).c_str(), std::to_wstring(chisten).size());
		chisten--;
		Sleep(10);
		QueryPerformanceCounter(&EnDec);

	}
	Sleep(10);
	ReleaseDC(hWnd, hdc);
	return 0;
}
DWORD Printdiss(PVOID x) {
	QueryPerformanceCounter(&StSub);
	HWND hWnd = (HWND)x;
	HDC hdc = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	int X = 200;
	int Y = 40;
	int i = 1;
	while (chisone - chisten < 10000) {
		if (chisone - chisten == -999)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		if (chisone - chisten == -99)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		if (chisone - chisten == -9)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		if (chisone - chisten == 999)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		if (chisone - chisten == 99)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		if (chisone - chisten == 9)
			PostMessage(hWnd, WM_APP_MY_THREAD_UPDATE, 0, (LPARAM)60);
		TextOut(hdc, 0, Y, L"Third Thread", std::wcslen(L"Third Thread"));
		TextOut(hdc, X, Y, std::to_wstring(chisone - chisten).c_str(), std::to_wstring(chisone - chisten).size());
		i++;
		Sleep(10);
		QueryPerformanceCounter(&EnSub);
	}
	ReleaseDC(hWnd, hdc);
	return 0;
}
void ReceiveTime(PVOID x) {
	HWND hWnd = (HWND)x;
	HDC hdc = GetDC(hWnd);
	RECT rect;
	FILETIME CreationTime;
	FILETIME ExitTime;
	FILETIME KernelTime;
	FILETIME UserTime;
	SYSTEMTIME   lpSystemTime;
	int X = 60;
	int Y = 40;
	static DWORD exCodeInc, exCodeSub, exCodeDec;
	while (1) {
		wchar_t str[32];
		std::wstring timeIncStr, timeDecStr, timeSubStr;

		double timeInc = (double)(EnInc.QuadPart - StInc.QuadPart) / Fraq.QuadPart;
		swprintf_s(str, L"%.2f", timeInc);
		timeIncStr = str;
		double timeDec = (double)(EnDec.QuadPart - StDec.QuadPart) / Fraq.QuadPart;
		memset(str, '\0', sizeof(str));
		swprintf_s(str, L"%.2f", timeDec);
		timeDecStr = str;
		double timeSub = (double)(EnSub.QuadPart - StSub.QuadPart) / Fraq.QuadPart;
		memset(str, '\0', sizeof(str));
		swprintf_s(str, L"%.2f", timeSub);
		timeSubStr = str;
		TextOut(hdc, 0, Y + 20, L"threadInc Time", std::wcslen(L"threadInc Time"));
		TextOut(hdc, X + 140, Y + 20, timeIncStr.c_str(), timeIncStr.size());
		TextOut(hdc, 0, Y + 40, L"threadDec Time", std::wcslen(L"threadInc Time"));
		TextOut(hdc, X + 140, Y + 40, timeDecStr.c_str(), timeDecStr.size());
		TextOut(hdc, 0, Y + 60, L"threadSub Time", std::wcslen(L"threadInc Time"));
		TextOut(hdc, X + 140, Y + 60, timeSubStr.c_str(), timeSubStr.size());
		Sleep(2);

	}
	ReleaseDC(hWnd, hdc);
}