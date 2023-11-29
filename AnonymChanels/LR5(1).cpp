#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

int main() {
    int sizeOfArray;
    cout << "Input size of array: ";
    cin >> sizeOfArray;

    HANDLE hWritePipe, hReadPipe;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

    STARTUPINFO stp;
    PROCESS_INFORMATION pi;
    ZeroMemory(&stp, sizeof(STARTUPINFO));
    stp.cb = sizeof(STARTUPINFO);
    wchar_t commandLine[80];
    wsprintf(commandLine, L"Simple.exe %d %d", (int)hWritePipe, (int)hReadPipe);
    CreateProcess(NULL, commandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &stp, &pi);

    DWORD dwBytesWritten;
    WriteFile(hWritePipe, &sizeOfArray, sizeof(int), &dwBytesWritten, NULL);

    vector<int> array(sizeOfArray);

    CloseHandle(hWritePipe);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwBytesRead;
    int sizeOfNewArray;
    ReadFile(hReadPipe, &sizeOfNewArray, sizeof(int), &dwBytesRead, NULL);
    vector<int> resultArray(sizeOfNewArray);


    cout << "Received Prime Numbers from Simple Process: ";
    for (int i = 0; i < sizeOfNewArray; ++i) {
        ReadFile(hReadPipe, &resultArray[i], sizeof(int), &dwBytesRead, NULL);
        cout << resultArray[i] << " ";
    }
    cout << endl;

    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
