#include <iostream>
#include <windows.h>

int main() {
    // Create anonymous pipe
    HANDLE hPipeRead, hPipeWrite;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0)) {
        std::cerr << "Error creating pipe" << std::endl;
        return 1;
    }

    // Prepare STARTUPINFO and PROCESS_INFORMATION structures
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    // Create Simple process
    if (!CreateProcess(L"Simple.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Error creating Simple process" << std::endl;
        return 1;
    }

    // Close unnecessary handles
    CloseHandle(pi.hThread);
    CloseHandle(hPipeRead);

    // Get the size of the array from the user
    int arraySize;
    std::cout << "Enter the size of the array: ";
    std::cin >> arraySize;

    // Pass the array size to Simple process
    WriteFile(hPipeWrite, &arraySize, sizeof(arraySize), NULL, NULL);

    // Wait for Simple process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(hPipeWrite);

    std::cout << "Press any key to exit Server..." << std::endl;
    getchar(); // Wait for user input before exiting
    return 0;
}
