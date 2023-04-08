#include"include.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include "api/auth.hpp"
#include <string>
#include "api/skStr.h"
#include "urlmon.h"
#define yeuem HWND
HWND hwnd;
DWORD PID;
HANDLE handle;
DWORD FindProcessId(const std::wstring& processName);
int emu;
int games;		
int ipadview;
int filesystem;
DWORD ProcPID;

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);
using namespace KeyAuth;
std::string name = "QuadraBlade Bypass"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "AHSGctOhPR"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "a1809eec42c4a6c4cc07491af33302fb902ee684e6305b2058fc1eb3bd7c17e1"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.7"; // leave alone unless you've changed version on website
std::string url = "https://keyauth.win/api/1.2/"; // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);
using namespace std;
string publicserver;
string key;
bool FileExits(string namefile) {
	fstream my_file;
	my_file.open(namefile, ios::out);
	if (!my_file) {
		return false;
	}
	else {
		return true;
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		
		HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		_ASSERTE(hIcon != 0);										 
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);		
		return 0;
	}
	case WM_COMMAND:
	{
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	return (DefWindowProc(hwnd, msg, wParam, lParam));
}
std::string GetCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}
bool Adb_Cmd(string CmdCode)
{
	if (!CmdCode.empty())	
	{
		string CmdCode2 = "/C " + CmdCode;
		std::wstring stemp = std::wstring(CmdCode2.begin(), CmdCode2.end());
		LPCWSTR sw = stemp.c_str();
		string CurrentDirectory = GetCurrentDirectory();
		std::wstring stemp2 = std::wstring(CurrentDirectory.begin(), CurrentDirectory.end());
		LPCWSTR sw2 = stemp2.c_str();
		SHELLEXECUTEINFO info1 = { 0 };
		info1.cbSize = sizeof(SHELLEXECUTEINFO);
		info1.fMask = SEE_MASK_NOCLOSEPROCESS;
		info1.hwnd = NULL;
		info1.lpVerb = NULL;
		info1.lpFile = L"cmd.exe";
		info1.lpParameters = sw;
		info1.lpDirectory = sw2;
		info1.nShow = SW_HIDE;
		info1.hInstApp = NULL;
		ShellExecuteEx(&info1);
		WaitForSingleObject(info1.hProcess, INFINITE);
		return true;
	}
	else
	{
		return false;
	}
}
bool runfile(LPCWSTR lpfile)
{

	SHELLEXECUTEINFO info1 = { 0 };
	info1.cbSize = sizeof(SHELLEXECUTEINFO);
	info1.fMask = SEE_MASK_NOCLOSEPROCESS;
	info1.hwnd = NULL;
	info1.lpVerb = NULL;
	info1.lpFile = lpfile;
	info1.lpParameters = NULL;
	info1.lpDirectory = NULL;
	info1.nShow = SW_HIDE;
	info1.hInstApp = NULL;
	ShellExecuteEx(&info1);
	WaitForSingleObject(info1.hProcess, INFINITE);
	return true;
}
bool RunHide(LPCWSTR lpfile)
{
	runfile(lpfile);
	std::future<bool> fut = std::async(runfile, lpfile);
	fut.wait();
}
bool cmd(string text) {
	std::future<bool> fut = std::async(Adb_Cmd, text);
	fut.wait();
}
void WriteResToDisk(std::string PathFile, LPCWSTR File_WITHARG)
{
	HRSRC myResource = ::FindResource(NULL, File_WITHARG, RT_RCDATA);
	unsigned int myResourceSize = ::SizeofResource(NULL, myResource);
	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
	void* pMyExecutable = ::LockResource(myResourceData);
	std::ofstream f(PathFile, std::ios::out | std::ios::binary);
	f.write((char*)pMyExecutable, myResourceSize);
	f.close();
}
void SetScreenBufferSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD NewSize;
	NewSize.X = width;
	NewSize.Y = height;

	SetConsoleScreenBufferSize(hStdout, NewSize);
}
/*DWORD GetProcId(const char* procName)
{
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hSnap == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(hSnap, &procEntry);
    if (!strcmp(procEntry.szExeFile, procName))
    {
        CloseHandle(hSnap);
        return procEntry.th32ProcessID;
    }
    while (Process32Next(hSnap, &procEntry))
    {
        if (!strcmp(procEntry.szExeFile, procName))
        {
            CloseHandle(hSnap);
            return procEntry.th32ProcessID;
        }
    }
    CloseHandle(hSnap);
    return 0;
}
*/
DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}
void runemulator()
{
	string emuC = "C:/Program Files/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuD = "D:/Program Files/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuE = "E:/Program Files/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuF = "F:/Program Files/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emucc = "C:/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emudd = "D:/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuee = "E:/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuff = "F:/TxGameAssistant/ui/AndroidEmulatorEx.exe";
	string emuPath;
	ifstream ifile;

	ifile.open(emuD);
	if (ifile) {
		string emuPath = "D:\\\"Program Files\"\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emuE);
	if (ifile) {
		string emuPath = "E:\\\"Program Files\"\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emuF);
	if (ifile) {
		string emuPath = "F:\\\"Program Files\"\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emucc);
	if (ifile) {
		string emuPath = "C:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emudd);
	if (ifile) {
		string emuPath = "D:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emuee);
	if (ifile) {
		string emuPath = "E:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emuff);
	if (ifile) {
		string emuPath = "F:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
	ifile.open(emuC);
	if (ifile) {
		string emuPath = "C:\\\"Program Files\"\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe";
		string fuck = "start " + emuPath + " -vm 100";
		const char* pp = fuck.c_str();
		cmd(fuck);
	}
	ifile.close();
}
int gettrueaow()
{
	int pid = 0;
	PROCESS_MEMORY_COUNTERS ProcMC;
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE ProcHandle;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &ProcEntry) == TRUE)
	{
		while (Process32Next(snapshot, &ProcEntry) == TRUE)
		{
			if (lstrcmpiW(ProcEntry.szExeFile, L"aow_exe.exe") == 0)
			{
				ProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcEntry.th32ProcessID);

				if (NULL == ProcHandle)
					continue;

				if (GetProcessMemoryInfo(ProcHandle, &ProcMC, sizeof(ProcMC)))
				{
					if (ProcMC.WorkingSetSize > 300000000)
					{
						pid = ProcEntry.th32ProcessID;
						return pid;
						break;
					}

				}

				CloseHandle(ProcHandle);
			}
		}
	}
	CloseHandle(snapshot);
}
void safexit();
void runemulator2();
void SetColor(WORD color)
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;	
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
// add DISCORS RPC here
inline bool FileExist(const std::string& name) {
	FILE* file = nullptr;
	errno_t err = fopen_s(&file, name.c_str(), "r");
	if (err == 0 && file != nullptr) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}
LONG GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
	}
	return nError;
}
void SetWindowSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Bottom = height - 1;
	WindowSize.Right = width - 1;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);
} 
std::string execute(std::string cmd)
{
	std::string file_name = "C:\\adb_log_server.txt";
	std::system((cmd + " > " + file_name).c_str());
	std::ifstream file(file_name);
	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	SHELLEXECUTEINFO info1 = { 0 };
	info1.nShow = SW_HIDE;
}
int main()
{
	std::cout << "DANG KET NOI TOI SERVER MIAO BU CAC..." << endl;
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		std::cout << "Status: " << KeyAuthApp.data.message << endl;
		Sleep(1500);
		exit(0);
	}
	SetWindowSize(80, 33);
	SetScreenBufferSize(80, 33);
	SetupDiscord();
	UpdateDiscord();
	//WriteResToDisk("C:\\Windows\\DiscordRPC.exe", MAKEINTRESOURCE(IDI_RCDATA18));
	system("cls");
	SetConsoleTitleA("BYPASS MIAO BU CAC SUC VAT CHIM BE");
	WriteResToDisk("C:\\Program Files\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("C:\\Program Files\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("F:\\Program Files\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("D:\\Program Files\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("E:\\Program Files\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("C:\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("D:\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("E:\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("F:\\TxGameAssistant\\ui\\adb.exe", MAKEINTRESOURCE(IDI_RCDATA12));
	WriteResToDisk("C:\\Program Files\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("F:\\Program Files\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("D:\\Program Files\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("E:\\Program Files\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("C:\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("D:\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("E:\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	WriteResToDisk("F:\\TxGameAssistant\\ui\\AdbWinApi.dll", MAKEINTRESOURCE(IDI_RCDATA13));
	SetColor(9);
	std::wcout << R"~(
  __  __ _____          ____      ____  _    _      _____          _____ 
 |  \/  |_   _|   /\   / __ \    |  _ \| |  | |    / ____|   /\   / ____|
 | \  / | | |    /  \ | |  | |   | |_) | |  | |   | |       /  \ | |     
 | |\/| | | |   / /\ \| |  | |   |  _ <| |  | |   | |      / /\ \| |     
 | |  | |_| |_ / ____ \ |__| |   | |_) | |__| |   | |____ / ____ \ |____ 
 |_|  |_|_____/_/    \_\____/    |____/ \____/     \_____/_/    \_\_____|
                                                                         
                                                                       
          DIT ME MAY MIAO, DIT CA HO NHA MAY, DIT CA LO NHA MAY.
    MIAO BU CAC VERSION: 2.5                  STATUS: MIAO BU CAC SAFE
                                 
)~" << endl;
	SetColor(11);
	if (FileExist(("C:\\DITMEMIAO.lic")))
	{
		ifstream fileIn("C:\\DITMEMIAO.lic");
		getline(fileIn, key);
		fileIn.close();
		KeyAuthApp.license(key);
		if (!KeyAuthApp.data.success)
		{
			SetColor(4);
			std::cout << "STATUS: CAC BE' QUA' MIAO DEO NHAN" << endl;
			cmd("del C:\\DITMEMIAO.lic");
			cmd("del DITMEMIAO.lic");
			cmd("del C:/REBUILD.lic");
			Sleep(1500);
			exit(0);
		}
		else if (KeyAuthApp.data.success)
		{
			std::cout << "DA TIM THA'Y CAC MIAO TUNG BU': " << key << std::endl;
			Sleep(500);
			std::cout << "MIAO DA NHAN BU CAC BAN THANH CONG!!!\n";
			Sleep(500);
			for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) { // Prompto#7895 was here
				auto sub = KeyAuthApp.data.subscriptions.at(i);
				std::cout << skCrypt("THOI GIAN BU' CAC CUA MIAO LA: ") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry))) << endl;
			}
		}


	}
	else
	{
		cout << "VUI LONG NHAP CHIM CHO MIAO BU': ";
		cin >> key;
		KeyAuthApp.license(key);
		if (!KeyAuthApp.data.success)
		{
			SetColor(4);
			std::cout << "STATUS: CAC BE' QUA' MIAO DEO NHAN" << endl;
			Sleep(1500);
			exit(0);

		}
		else if (KeyAuthApp.data.success)
		{
			SetColor(1);
			std::cout << "MIAO DA NHAN BU CAC BAN THANH CONG!!! \n";
			Sleep(500);
			for (int i = 0; i < KeyAuthApp.data.subscriptions.size(); i++) { // Prompto#7895 was here
				auto sub = KeyAuthApp.data.subscriptions.at(i);
				std::cout << skCrypt("THOI GIAN BU' CAC CUA MIAO LA:") << tm_to_readable_time(timet_to_tm(string_to_timet(sub.expiry))) << endl;
			}
			ofstream outFile("C:/DITMEMIAO.lic");
			ofstream HaxKey("C:/REBUILD.lic");
			outFile << key;
			HaxKey << key;
			outFile.close();

		}


	}
	SetColor(2);
	cout << "===================================================================\n";
	SetColor(9);
	string omg11 = "HAY CHON SERVER DE MIAO BU' CAC: 1.VNG || 2.Global || 3.Korea || 4.Taiwan\n";
	int v211 = 0;
	while (omg11[v211] != '\0')
	{
		cout << omg11[v211];
		Sleep(10);
		v211++;
	};
	int chongem;
	cin >> chongem;
	if (chongem == 1)
	{
		publicserver = "com.vng.pubgmobile";
	}
	else if (chongem == 2)
	{
		publicserver = "com.tencent.ig";
	}
	else if (chongem == 3)
	{
		publicserver = "com.pubg.krmobile";
	}
	else if (chongem == 4)
	{
		publicserver = "com.rekoo.pubgm";
	}
	else
	{
		SetColor(4);
		cout << "BAN DA NHAP SAI MIAO SE TU NGUNG BU' CAC...";
		Sleep(3000);
		exit(0);
	}
	SetColor(14);
	WriteResToDisk("C:\\Program Files\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("F:\\Program Files\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("D:\\Program Files\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("E:\\Program Files\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("C:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("D:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	WriteResToDisk("E:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));               
	WriteResToDisk("F:\\TxGameAssistant\\ui\\AndroidEmulatorEx.exe", MAKEINTRESOURCE(IDI_RCDATA7));
	Sleep(1000);
	string hello198 = "[-] DA CAI DAT GIA LAP CHO MIAO BU CAC THANH CONG";                                                 
	int v = 0;                             
	while (hello198[v] != '\0')
	{            
		cout << hello198[v];                                                                                                                                                                             
		Sleep(5);
		v++;
	};
	Sleep(1000);                                                                                              
	SetColor(11);
	string hello1 = "\n[-] KHOI DONG CAC CHO MIAO BU'...";
	int s = 0;
	while (hello1[s] != '\0')
		
	{
		
		cout << hello1[s];
		
		Sleep(5);
		
		s++;
		
	};
	runemulator2();
	Sleep(8000);
	string ProcName = "AndroidEmulatorEx.exe";
	std::wstring wStrProcName;
	wStrProcName.assign(ProcName.begin(), ProcName.end());
	ProcPID = FindProcessId(wStrProcName);
	if (ProcPID == 0)
	{
		
		SetColor(4);
		cout << "\n[!] MIAO DANG CHE^ CAC BAN BE'";
		SetColor(10);
		cout << "\n[!] VUI LONG DOI THAY CAC MOI TO HON CHO MIAO BU'...";
		runemulator();
		Sleep(10000);
		string ProcName1 = "AndroidEmulatorEx.exe";
		std::wstring wStrProcName1;
		wStrProcName1.assign(ProcName1.begin(), ProcName1.end());
		ProcPID = FindProcessId(wStrProcName1);
		Sleep(30);
		if (ProcPID == 0)
		{
			SetColor(4);
			cout << "\n[!] MIAO CHE^ BAN CAC BE' [!]";
			Sleep(3000);
			exit(0);
		}
		else
		{
			SetColor(14);
			cout << "\nDA TIM THAY CHIM CUA MIAO VOI PID: " << ProcPID;
		}
	}
	else
	{
		
		SetColor(14);
		cout << "\nDA TIM THAY CHIM CUA MIAO VOI PID: " << ProcPID;
		
	}
	HRESULT hr1 = URLDownloadToFile(NULL, L"https://cdn.discordapp.com/attachments/1007113999860629525/1093517951526780958/libhelpshiftlistener.so", L"C:\\Windows\\Fonts\\libigshare.so", 0, NULL);
	SetColor(11);
	Sleep(10000);
	string hello1b = "\nVUI LONG CHO` MIAO BU' CAC\n";
	int b = 0;
	while (hello1b[b] != '\0')
	{
		cout << hello1b[b];
		Sleep(30);
		b++;
	};
	cmd("adb kill-server");
	cmd("adb startl-server");
	cmd("adb.exe -s emulator-5554 shell rm -rf /data/data/" + publicserver + "/databases");
	cmd("adb.exe -s emulator-5554 shell am start -n " + publicserver + "/com.epicgames.ue4.SplashActivity");
	execute("adb shell rm - rf /data/data/" + publicserver + "/lib/libigshare.so");
	execute("adb push C:\\Windows\\Fonts\\libigshare.so /data/data/" + publicserver + "/lib/libigshare.so");
	int truepid2;
	while (true)
	{	
		truepid2 = gettrueaow();	
		if (truepid2 > 2)
		{
			break;
		}
	}
	SetColor(1);
	string hello2312 = "MIAO DANG BU' CAC...";
	int c2 = 0;
	while (hello2312[c2] != '\0')
	{
		cout << hello2312[c2];
		Sleep(30);
		c2++;
	};
	Sleep(1000);
	SetColor(14);
	string hello23121 = "\nMIAO BU CAC THANH CONG BAN CO THE CHOI GAME";
	int c21 = 0;
	while (hello23121[c21] != '\0')
	{
		cout << hello23121[c21];
		Sleep(30);
		c21++;
	};
	cmd("DEL C:\\Windows\\Fonts\\libigshare.so");
	SetColor(2);
	cout << "\n===================================================================\n";
	SetColor(4);
	string hello2 = "NEU MIAO KHONG CHIU BU' CAC BAN CO THE CHAY LAI FILE";
	int c = 0;
	while (hello2[c] != '\0')
	{
		cout << hello2[c];
		Sleep(30);
		c++;
	};
	cout << "\nNHAN PHI'M 1 DE MIAO NGUNG BU CAC\n";
	cin >> emu;
	if (emu == 1)
	{
		safexit();
	}
	else if (!emu)
	{
		SetColor(4);
		cout << "BAN DA NHAP SAI MIAO SE TU NGUNG BU' CAC...";
		Sleep(3000);
		exit(0);
	}

}


void runemulator2() //vnhaxbp no unicode multi singe
{
	HKEY hKey;
	LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Tencent\\MobileGamePC\\UI", 0, KEY_READ, &hKey);
	bool bExistsAndSuccess(lRes == ERROR_SUCCESS);
	bool bDoesNotExistsSpecifically(lRes == ERROR_FILE_NOT_FOUND);
	std::wstring strValueOfBinDir;
	GetStringRegKey(hKey, L"InstallPath", strValueOfBinDir, L"bad");
	strValueOfBinDir = strValueOfBinDir + L"\\AndroidEmulatorEx.exe";
	ShellExecute(NULL, L"open", strValueOfBinDir.c_str(), L" -vm 100", NULL, SW_SHOWDEFAULT);
}

void safexit()
{
	SetColor(3);
	cout << "Dang dong gameloop...";
	cmd("TaskKill /F /IM appmarket.exe");
	cmd("TaskKill /F /IM AndroidEmulator.exe");
	cmd("TaskKill /F /IM AndroidEmulatorEx.exe");
	cmd("TaskKill /F /IM AndroidEmulatorEn.exe");
	cmd("TaskKill /F /IM ProjectTitan.exe");
	cmd("TaskKill /F /IM aow_exe.exe");
	cmd("TaskKill /F /IM QMEmulatorService.exe");
	cmd("taskkill /f /im RuntimeBroker.exe");
	cmd("taskkill /f /im adb.exe");
	cmd("taskkill /f /im syzs_dl_svr.exe");
	cmd("taskkill /f /im GameLoader.exe");
	cmd("taskkill /f /im TSettingCenter.exe");
	cmd("taskkill /f /im checker.exe");
	cmd("taskkill /f /im TxGaDcc.exe");
	cmd("taskkill /f /im Synaptics.exe");
	cmd("taskkill /f /im cheatengine-x86_64-SSE4-AVX2.exe");
	cmd("net stop aow_drv");
	cmd("net stop Tensafe");
	cmd("netsh winsock reset");
	cmd("rd %temp% /s /q ");
	cmd("md %temp%");
	cmd("del /s /f /q C:\\Windows\\Prefetch\\*.*");
	cmd("del /s /f /q C:\\Windows\\temp\\*.*");
	cmd("del /s /f /q C:\\Windows\\cmd32\\temp\\*.*");
	cmd("del /s /f /q %USERPROFILE%\\appdata\\local\\temp\\*.*");
	cmd("del /q C:\\aow_drv.log");
	cmd("del /q C:\\Windows\\Venom.sys");
	cmd("sc stop venomes");
	cmd("sc delete venomes");
	cmd("DEL C:\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL D:\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL E:\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL F:\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL C:\\Program Files\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL D:\\Program Files\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL E:\\Program Files\\TxGameAssistant\\ui\\dsound.dll");
	cmd("DEL F:\\Program Files\\TxGameAssistant\\ui\\dsound.dll");
	cmd("taskkill / f / im DiscordRPCActive.exe");
	cmd("taskkill /f  /im RPCBlock.exe");
	Sleep(1000);

	SetColor(19);
	string hello2 = "\nDang xoa DSOUND.....";
	int c = 0;
	while (hello2[c] != '\0')
	{
		cout << hello2[c];
		Sleep(60);
		c++;
	};
	cmd("cd C:/Program Files/TxGameAssistant/ui/dsound.dll");
	cmd("DEL dsound.dll");
	cmd("cd D:/Program Files/TxGameAssistant/ui/dsound.dll");
	cmd("DEL dsound.dll");
	cmd("cd E:/Program Files/TxGameAssistant/ui/dsound.dll");
	cmd("DEL dsound.dll");
	cmd("cd F:/Program Files/TxGameAssistant/ui/dsound.dll");
	cmd("DEL dsound.dll");
	cmd("DEL C:/TxGameAssistant/ui/dsound.dll");
	cmd("DEL D:/TxGameAssistant/ui/dsound.dll");
	cmd("DEL E:/TxGameAssistant/ui/dsound.dll");
	cmd("DEL F:/TxGameAssistant/ui/dsound.dll");
	cmd("DEL \"C:/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"D:/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"E:/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"F:/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"C:/Program Files/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"D:/Program Files/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"E:/Program Files/TxGameAssistant/ui/dsound.dll\"");
	cmd("DEL \"F:/Program Files/TxGameAssistant/ui/dsound.dll\"");
	string hoho = "C:/TxGameAssistant/ui/dsound.dll";
	string hoho1 = "D:/TxGameAssistant/ui/dsound.dll";
	string hoho2 = "E:/TxGameAssistant/ui/dsound.dll";
	string hoho3 = "F:/TxGameAssistant/ui/dsound.dll";
	string concacc1 = "DEL " + hoho;
	//system(concacc1);
	//string concacc2 = "DEL " + hoho1;
	//system(concacc2);
	//string concacc3 = "DEL " + hoho2;
	//system(concacc3);
	//string concacc4 = "DEL " + hoho3;
	//system(concacc4);
	exit(0);
}
std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
