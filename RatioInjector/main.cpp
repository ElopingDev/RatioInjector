#include "injection.h"

const char szDllFile[] = "C:\\Users\\elopi\\Desktop\\Folders\\Dev\\Projects\\RatioHook\\Release\\RatioHook.dll";
const char szProc[] = "csgo.exe";

int main()
{
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		DWORD Err = GetLastError();
		printf("CreateToolhelp32Snpashot failed: 0x%X\n", Err);
		system("PAUSE");
		return 0;
	}

	DWORD PID = 0;
	BOOL bRet = Process32First(hSnapshot, &PE32);
	while (bRet)
	{
		if (!strcmp(szProc, PE32.szExeFile))
		{
			PID = PE32.th32ProcessID;
			break;
		}

		bRet = Process32Next(hSnapshot, &PE32);
	}
	CloseHandle(hSnapshot);
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc)
	{
		DWORD Err = GetLastError();
		printf("OpenProcess failed: 0x%X\n, Err");
		system("PAUSE");
		return 0;
	}

	if (!ManualMap(hProc, szDllFile))
	{
		CloseHandle(hProc);
		printf("Something went wrong\n");
		system("PAUSE");
			return 0;
	}

	CloseHandle(hProc);

	return 0;
}