#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: Reporter <binary_filename> <report_filename> <hourly_rate>" << endl;
        return 1;
    }

    const char* binaryFilename = argv[1];
    const char* reportFilename = argv[2];
    double hourlyRate = atof(argv[3]);

    ifstream binaryFile(binaryFilename, ios::binary);
    if (!binaryFile.is_open()) {
        cout << "Error opening binary file." << endl;
        return 1;
    }

    ofstream reportFile(reportFilename);
    if (!reportFile.is_open()) {
        cout << "Error opening report file." << endl;
        return 1;
    }

    reportFile << "Отчет по файлу \"" << binaryFilename << "\"" << endl;
    reportFile << "Номер сотрудника, имя сотрудника, часы, зарплата" << endl;

    employee emp;
    while (binaryFile.read((char*)&emp, sizeof(employee))) {
        double salary = emp.hours * hourlyRate;
        reportFile << emp.num << ", " << emp.name << ", " << emp.hours << ", " << fixed << setprecision(2) << salary << endl;
    }

    binaryFile.close();
    reportFile.close();
    cout << "Report file " << reportFilename << " created successfully." << endl;

    return 0;
}
