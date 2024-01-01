#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <conio.h>

const int EMPLOYEE_DATA_SIZE = 100;

int employeeNum[EMPLOYEE_DATA_SIZE];
char employeeNames[EMPLOYEE_DATA_SIZE][10];
double employeeHours[EMPLOYEE_DATA_SIZE];

int empCmp(const void* p1, const void* p2) {
    return (*(int*)p1 - *(int*)p2);
}

const int COMMAND_LENGTH = 12;
const int MS_PIPE_WAIT = 2000;
const std::string IP_NAME = "START_ALL";
const std::string PIPE_NAME = "\\\\.\\pipe\\pipe_name";

void errorMessage(std::string message) {
    std::cerr << message;
    getch();
}

void printEmployee(int index) {
    std::cout << "ID: " << employeeNum[index] << "\tName: " << employeeNames[index]
              << "\tHours: " << employeeHours[index] << '\n';
}

void messaging(HANDLE handlePipe) {
    std::cout << "If you want to quit, press Ctrl+Z...\n";

    while (true) {
        std::cout << "Input r to read or w to write command and ID of employee: ";
        char command[COMMAND_LENGTH];
        std::cin.getline(command, COMMAND_LENGTH, '\n');

        if (std::cin.eof()) {
            errorMessage("File is empty... \n");
            return;
        }

        bool isSent;
        DWORD bytesWritten;
        isSent = WriteFile(handlePipe, command, COMMAND_LENGTH,
                           &bytesWritten, NULL);

        if (!isSent) {
            errorMessage("Message cannot be sent...\n");
            return;
        }

        bool isRead;
        DWORD readBytes;
        int tempEmployeeNum;
        char tempEmployeeName[10];
        double tempEmployeeHours;

        isRead = ReadFile(handlePipe, &tempEmployeeNum,
                          sizeof(int), &readBytes, NULL);

        if (!isRead) {
            errorMessage("Error in receiving answer...\n");
            continue;
        }

        if (tempEmployeeNum < 0) {
            errorMessage("Employee not found or being modified.\n");
            continue;
        }

        isRead = ReadFile(handlePipe, &tempEmployeeName,
                          sizeof(tempEmployeeName), &readBytes, NULL);

        if (!isRead) {
            errorMessage("Error in receiving answer...\n");
            continue;
        }

        isRead = ReadFile(handlePipe, &tempEmployeeHours,
                          sizeof(double), &readBytes, NULL);

        if (!isRead) {
            errorMessage("Error in receiving answer...\n");
            continue;
        }

        employeeNum[0] = tempEmployeeNum;
        strcpy(employeeNames[0], tempEmployeeName);
        employeeHours[0] = tempEmployeeHours;

        printEmployee(0);

        if ('w' == command[0]) {
            std::cout << "Input ID of employee: ";
            std::cin >> tempEmployeeNum;
            std::cout << "Input name of employee: ";
            std::cin >> tempEmployeeName;
            std::cout << "Input working hours of employee: ";
            std::cin >> tempEmployeeHours;

            std::cin.ignore(2, '\n');

            isSent = WriteFile(handlePipe, &tempEmployeeNum,
                               sizeof(int), &bytesWritten, NULL);

            if (!isSent) {
                errorMessage("Error in sending...\n");
                break;
            }

            isSent = WriteFile(handlePipe, &tempEmployeeName,
                               sizeof(tempEmployeeName), &bytesWritten, NULL);

            if (!isSent) {
                errorMessage("Error in sending...\n");
                break;
            }

            isSent = WriteFile(handlePipe, &tempEmployeeHours,
                               sizeof(double), &bytesWritten, NULL);

            if (!isSent) {
                errorMessage("Error in sending...\n");
                break;
            }

            std::cout << "New record has been sent.\n";
        }
    }
}

int main() {
    HANDLE handleReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, "READY_EVENT_1");
    HANDLE handleStartEvent = OpenEvent(SYNCHRONIZE, FALSE, IP_NAME.c_str());

    if (NULL == handleStartEvent || NULL == handleReadyEvent) {
        std::cerr << "Error in action with event...\n";
        getch();
        return GetLastError();
    }

    SetEvent(handleReadyEvent);
    WaitForSingleObject(handleStartEvent, INFINITE);
    HANDLE handlePipe;

    std::cout << "Process is started.\n";

    while (true) {
        handlePipe = CreateFile(PIPE_NAME.c_str(), GENERIC_WRITE | GENERIC_READ,
                                FILE_SHARE_READ, NULL,
                                OPEN_EXISTING, 0, NULL);

        const bool FLAG = WaitNamedPipe(PIPE_NAME.c_str(), MS_PIPE_WAIT)
                        + (INVALID_HANDLE_VALUE != handlePipe);

        if (FLAG) {
            if (INVALID_HANDLE_VALUE != handlePipe) {
                break;
            }
            errorMessage("timed out...\n");
            return 0;
        }
    }
    std::cout << "Pipe has been connecting...\n";
    messaging(handlePipe);
    return 0;
}
