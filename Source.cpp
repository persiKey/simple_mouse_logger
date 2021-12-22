#include "Windows.h"
#include "stdio.h"

namespace MyMouseHook
{
	HHOOK hook;
	FILE* MyFile;
	CHAR name[1024];
	HWND ActiveWindow;
	SYSTEMTIME Time;
	LRESULT _stdcall HookMouse(int code, WPARAM wparam, LPARAM lparam);
	void write_stat_to_file(const CHAR* str, MOUSEHOOKSTRUCT* a);
}

int main()
{
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);	

	if (!(SetWindowsHookEx(WH_MOUSE_LL, MyMouseHook::HookMouse, NULL, 0)))
	{
		MessageBox(NULL, L"Something went wrong", L"ERROR", MB_ICONERROR);
	}

	MSG message;
	while (true)
	{
		GetMessage(&message, NULL, 0, 0);
	}

	return 0;
}

void MyMouseHook::write_stat_to_file(const CHAR* str, MOUSEHOOKSTRUCT* a)
{
	fopen_s(&MyFile, "log.txt", "a+");
	if (MyFile == NULL)
	{
		MessageBox(NULL, L"Something went wrong", L"ERROR", MB_ICONERROR);
		return;
	}
	ActiveWindow = GetForegroundWindow();
	GetWindowTextA(ActiveWindow, name, 1023);
	GetLocalTime(&Time);

	fprintf(MyFile, "[X: %04d Y: %04d] [%02d:%02d] [%s] %s\n", a->pt.x, a->pt.y, Time.wHour, Time.wMinute, name, str);
	fclose(MyFile);
}

LRESULT _stdcall MyMouseHook::HookMouse(int code, WPARAM wparam, LPARAM lparam)
{
	if (code >= 0)
	{
		switch (wparam)
		{
		case WM_LBUTTONDOWN:write_stat_to_file("LEFT BUTTON", (MOUSEHOOKSTRUCT*)lparam); break;
		case WM_RBUTTONDOWN:write_stat_to_file("RIGHT BUTTON", (MOUSEHOOKSTRUCT*)lparam); break;
		case WM_MBUTTONDOWN:write_stat_to_file("MIDDLE BUTTON", (MOUSEHOOKSTRUCT*)lparam); break;
		default:
			break;
		}
	}
	return CallNextHookEx(hook, code, wparam, lparam);
}