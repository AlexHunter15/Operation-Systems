#include <iostream>
#include <windows.h>
#include <string>

int main() {
    setlocale(LC_ALL, "Russian");

    SetConsoleTitle(L"Parent Window");

    int n;
    std::cout << "Введите количество элементов: ";
    std::cin >> n;

    std::string Command = "LR2(2).exe " + std::to_string(n);

    for (int i = 0; i < n; ++i) {
        int element;
        std::cout << "Введите элемент " << i + 1 << ": ";
        std::cin >> element;
        Command += " " + std::to_string(element);
    }

    const char* command = Command.c_str();
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);

    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWMINIMIZED;
    si.cb = sizeof(STARTUPINFO);
    si.lpTitle = (LPWSTR)L"Child";
    int len = strlen(command) + 1;
    int size = MultiByteToWideChar(CP_UTF8, 0, command, len, NULL, 0);
    wchar_t* wCommand = new wchar_t[size];

    if (CreateProcess(NULL, (LPWSTR)wCommand, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    else {
        std::cerr << "Ошибка при создании процесса Child" << std::endl;
        return 1;
    }
}
