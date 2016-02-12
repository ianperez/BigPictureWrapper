#include <windows.h>

bool IsBigPictureOpen()
{
	return FindWindowEx(NULL, NULL, L"CUIEngineWin32", NULL) != NULL;
}

void CloseNvCurtain()
{
	HWND hWnd = FindWindowEx(NULL, NULL, L"NvCurtain", NULL);
	if (hWnd)
	{
		DWORD dwProcessId;
		if (GetWindowThreadProcessId(hWnd, &dwProcessId) > 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}
	}
}

int ElevateToAdmin()
{
	ShellExecute(NULL, L"runas", L"BigPictureService.exe", L"service", NULL, SW_NORMAL);

	return 0;
}

int ServiceMain()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"BigPictureService");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return 0;

	while (1)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		if (DeleteFile(L"CloseNvCurtain"))
			CloseNvCurtain();

		if (DeleteFile(L"CloseService"))
			break;

		Sleep(1000);
	}

	CloseHandle(hMutex);

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
	WCHAR lpFilename[MAX_PATH + 1];
	GetModuleFileName(NULL, lpFilename, MAX_PATH + 1);

	if (wcsstr(lpFilename, L"BigPictureService.exe"))
	{
		if (lstrlen(lpCmdLine) == 0)
			return ElevateToAdmin();
		if (!lstrcmpi(lpCmdLine, L"service"))
			return ServiceMain();
	}
	else if (wcsstr(lpFilename, L"BigPictureWrapper.exe"))
	{
		// Open Big Picture
		ShellExecute(NULL, NULL, L"steam://open/bigpicture", NULL, NULL, SW_NORMAL);

		// Wait for it to open
		while (!IsBigPictureOpen())
			Sleep(1000);

		// Wait for it to close
		while (IsBigPictureOpen())
			Sleep(1000);

		// Signal service to close the NVIDIA GameStream Curtain
		CloseHandle(CreateFile(L"CloseNvCurtain", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL));
	}

	return 0;
}
