#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <sstream>
using namespace std;

HANDLE start_process(wstring command_line) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, const_cast<LPWSTR>(command_line.c_str()), NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        return NULL;
    }

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

int main() {
    wstring file_name;
    wcout << L"Enter file name:";
    wcin >> file_name;
    fstream file(file_name.c_str(), ios::binary | ios::out);
    int number_of_records;
    wcout << L"Enter number of records:";
    cin >> number_of_records;
    if (!file.is_open()) {
        wcout << L"Error while creating file!";
        return 0;
    }
    file.close();

    int number_of_senders;
    wcout << L"Enter number of Senders:";
    cin >> number_of_senders;

    HANDLE* senders = new HANDLE[number_of_senders];
    HANDLE* events = new HANDLE[number_of_senders];

    HANDLE mutex = CreateMutex(NULL, FALSE, L"mutex");
    HANDLE write_sem = CreateSemaphore(NULL, number_of_records, number_of_records, L"write_sem");
    HANDLE read_sem = CreateSemaphore(NULL, 0, number_of_records, L"read_sem");
    if (!mutex || !write_sem || !read_sem) {
        wcout << L"Error";
        return -1;
    }

    for (int i = 0; i < number_of_senders; ++i) {
        wstringstream ss;
        ss << L"Sender.exe " << file_name << L" " << number_of_records << L" " << i;

        HANDLE event = CreateEvent(NULL, FALSE, FALSE, (ss.str() + L"ready").c_str());
        events[i] = event;
        senders[i] = start_process(ss.str());
        if (senders[i] == NULL) {
            wcout << L"Error while creating process";
            return -1;
        }
    }

    WaitForMultipleObjects(number_of_senders, events, TRUE, INFINITE);

    int action = 1;

    while (true) {
        wcout << L"1 to read message\n";
        wcout << L"0 to exit\n";
        cin >> action;
        if (action != 0 && action != 1) {
            wcout << L"Unknown command";
            continue;
        }
        if (action == 0) {
            break;
        }
        WaitForSingleObject(read_sem, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        file.open(file_name.c_str(), ios::binary | ios::in);

        // Read message from the file as a string
        string mes;
        getline(file, mes);

        wcout << L"New message:" << mes.c_str() << L"\n";
        vector<string> file_text;
        string line;
        while (getline(file, line)) {
            file_text.push_back(line);
        }
        file.close();
        file.open(file_name.c_str(), ios::binary | ios::out);

        // Write updated data back to the file
        for (int i = 0; i < file_text.size(); ++i) {
            file << file_text[i].c_str() << "\n";
        }
        file.close();
        ReleaseMutex(mutex);
        ReleaseSemaphore(write_sem, 1, NULL);
    }

    for (int i = 0; i < number_of_senders; ++i) {
        CloseHandle(events[i]);
        CloseHandle(senders[i]);
    }

    delete[] events;
    delete[] senders;
    CloseHandle(mutex);
    CloseHandle(read_sem);
    CloseHandle(write_sem);
    return 0;
}
