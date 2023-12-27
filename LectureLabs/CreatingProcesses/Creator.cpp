#include <iostream>
#include <fstream>
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: Creator <filename> <number_of_records>" << endl;
        return 1;
    }

    const char* filename = argv[1];
    int numRecords = atoi(argv[2]);

    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return 1;
    }

    for (int i = 0; i < numRecords; i++) {
        employee emp;
        cout << "Enter employee number: ";
        cin >> emp.num;
        cout << "Enter employee name (up to 10 characters): ";
        cin >> emp.name;
        cout << "Enter hours worked: ";
        cin >> emp.hours;
        file.write((char*)&emp, sizeof(employee));
    }

    file.close();
    cout << "File " << filename << " created successfully." << endl;

    return 0;
}
