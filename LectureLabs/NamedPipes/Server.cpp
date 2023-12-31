#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <process.h>
#include <time.h>
#include <conio.h>
#include <algorithm>
#include <string>

const std::string pipeName = "\\\\.\\pipe\\pipe_name";
const int BUFF_LENGTH = 10;
const int MESSAGE_LENGTH = 10;

std::vector<int> empNums;
std::vector<std::string> empNames;
std::vector<double> empHours;

CRITICAL_SECTION empsCS;
bool* empIsModifying;

int generateCountOfClient() {
    srand(time(0));
    return (rand() / 5 + 3) % 5 + 2;
}

void readDataFromStd() {
    int empCount;
    std::cout << "Input count of employees: ";
    std::cin >> empCount;

    for (int i = 0; i < empCount; ++i) {
        int id;
        std::string name;
        double hours;

        std::cout << "Enter ID, name, and working hours of employee " << (i + 1) << ": ";
        std::cin >> id >> name >> hours;

        empNums.push_back(id);
        empNames.push_back(name);
        empHours.push_back(hours);
    }
}

void writeDataToFile(std::string fileName) {
    std::ofstream fout(fileName.c_str(), std::ios::out | std::ios::binary);

    for (size_t i = 0; i < empNums.size(); ++i) {
        fout.write(reinterpret_cast<const char *>(&empNums[i]), sizeof(int));
        fout.write(empNames[i].c_str(), empNames[i].size() + 1); // Include null terminator
        fout.write(reinterpret_cast<const char *>(&empHours[i]), sizeof(double));
    }

    std::cout << "Data has been written.\n";
    fout.close();
}

void sortEmps() {
    std::vector<std::pair<int, std::pair<std::string, double>>> employees;

    for (size_t i = 0; i < empNums.size(); ++i) {
        employees.push_back({ empNums[i], { empNames[i], empHours[i] } });
    }

    std::sort(employees.begin(), employees.end());

    for (size_t i = 0; i < empNums.size(); ++i) {
        empNums[i] = employees[i].first;
        empNames[i] = employees[i].second.first;
        empHours[i] = employees[i].second.second;
    }
}

void startPocesses(const int COUNT){
    for(int i = 0; i < COUNT; ++i) {
        std::string cmdArgs = "..\\..\\Client\\cmake-build-debug\\Client.exe ";
        std::string eventName = "READY_EVENT_";
        itoa(i + 1, buff, BUFF_LENGTH);

        eventName += buff;
        cmdArgs += eventName;

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        si.cb = sizeof(STARTUPINFO);
        ZeroMemory(&si, sizeof(STARTUPINFO));

        handleReadyEvents[i] = CreateEvent(NULL, TRUE,
                                      FALSE, eventName.c_str());

        char tempArg[60];
        strcat(tempArg, cmdArgs.c_str());

        if (CreateProcess(NULL, tempArg,
                           NULL, NULL,
                           FALSE, CREATE_NEW_CONSOLE,
                           NULL, NULL,
                           &si, &pi) == false) {
            std::cout << "Creation process error.\n";

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

DWORD WINAPI messaging(LPVOID p){
    employee* errorEmp = new employee;
    errorEmp->num = -1;

    HANDLE hPipe = (HANDLE)p;
    while(true){
        DWORD readBytes;
        char message[MESSAGE_LENGTH];

        bool isRead = ReadFile(hPipe, message,
                               MESSAGE_LENGTH, &readBytes, NULL);

        if(false == isRead){
            if(ERROR_BROKEN_PIPE == GetLastError()){
                std::cout << "Client disconnected." << std::endl;
            }
            else {
                std::cerr << "Error in reading a message." << std::endl;
            }
            break;
        }

        if(strlen(message) != 0) {
            char command = message[0];
            message[0] = ' ';
            int id = atoi(message);

            DWORD bytesWritten;
            EnterCriticalSection(&empsCS);
            employee* empToSend = findEmp(id);
            LeaveCriticalSection(&empsCS);

            if(NULL == empToSend){
                empToSend = errorEmp;
            }
            else{
                int ind = empToSend - emps;

                if(empIsModifying[ind]) {
                    empToSend = errorEmp;
                }
                else{
                    if(command == 'r') {
                        std::cout << "Requested to read ID " << id << ".";
                    } else if(command == 'w') {
                        std::cout << "Requested to modify ID " << id << ".";
                        empIsModifying[ind] = true;
                    } else {
                        std::cout << "Unknown request. ";
                    }
                }
            }
            bool isSent = WriteFile(hPipe, empToSend,
                                    employeeSize, &bytesWritten, NULL);
            if(true == isSent) {
                std::cout << "Answer is sent.\n";
            }
            else {
                std::cout << "Error in sending answer.\n";
            }

            if(empToSend != errorEmp && 'w' == command){
                isRead = ReadFile(hPipe, empToSend,
                                  employeeSize, &readBytes, NULL);
                if(false == isRead){
                    std::cerr << "Error in reading a message." << std::endl;
                    break;
                }
                else{
                    std::cout << "Employee record changed.\n";

                    empIsModifying[empToSend - emps] = false;
                    EnterCriticalSection(&empsCS);
                    sortEmps();
                    LeaveCriticalSection(&empsCS);
                }
            }
        }
    }
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    delete errorEmp;
    return 0;
}

void openPipes(int clientCount){
    HANDLE hPipe;
    HANDLE* hThreads = new HANDLE[clientCount];
    for(int i = 0; i < clientCount; ++i){
        hPipe = CreateNamedPipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX,
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                PIPE_UNLIMITED_INSTANCES,0,
                                0,INFINITE, NULL);
        if(!ConnectNamedPipe(hPipe, NULL)){
            std::cout << "No connected clients.\n";
            break;
        }
        if (INVALID_HANDLE_VALUE == hPipe){
            std::cerr << "Create named pipe failed.\n";
            getch();
            return;
        }
        hThreads[i] = CreateThread(NULL, 0,
                                   messaging, (LPVOID)hPipe,
                                   0,NULL);
    }
    std::cout << "Clients connected to pipe.\n";
    WaitForMultipleObjects(clientCount, hThreads, TRUE, INFINITE);
    std::cout << "All clients are disconnected.\n";
    delete[] hThreads;
}
int main() {
    std::string filename;
    std::cout << "Input name of file: ";
    std::cin >> filename;

    readDataFromStd();
    writeDataToFile(filename);
    sortEmps();

    int countOfClient = generateCountOfClient();
    empIsModifying = new bool[empNums.size()];

    for (size_t i = 0; i < empNums.size(); ++i) {
        empIsModifying[i] = false;
    }

    InitializeCriticalSection(&empsCS);
       HANDLE handleStartALL = CreateEvent(NULL, TRUE,
                                        FALSE, "START_ALL");

    handleReadyEvents = new HANDLE[countOfClient];
    startPocesses(countOfClient);

    WaitForMultipleObjects(countOfClient, handleReadyEvents,
                           TRUE, INFINITE);
    std::cout << "All processes are ready.Starting.\n";

    SetEvent(handleStartALL);

    openPipes(countOfClient);

    for(int i = 0; i < empCount; ++i) {
        emps[i].print();
    }

    std::cout << "Press any key to exit...\n";
    getch();
    DeleteCriticalSection(&empsCS);

    delete[] empIsModifying;
    delete[] handleReadyEvents;
    delete[] emps;
    return 0;
}
