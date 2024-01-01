#include <iostream>
#include <fstream>
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <vector>

using namespace std;
namespace bp = boost::process;
namespace asio = boost::asio;

int main() {
    wstring file_name;
    cout << "Enter file name:";
    wcin >> file_name;
    fstream file(file_name, ios::binary | ios::out);
    int number_of_records;
    cout << "Enter number of records:";
    cin >> number_of_records;
    if (!file.is_open()) {
        cout << "Error while creating file!";
        return 0;
    }
    file.close();

    int number_of_senders;
    cout << "Enter number of Senders:";
    cin >> number_of_senders;

    asio::io_context io_context;
    bp::group process_group;
    vector<bp::child> senders;

    HANDLE mutex = CreateMutex(NULL, FALSE, L"mutex");
    HANDLE write_sem = CreateSemaphore(NULL, number_of_records, number_of_records, L"write_sem");
    HANDLE read_sem = CreateSemaphore(NULL, 0, number_of_records, L"read_sem");
    if (!mutex || !write_sem || !read_sem) {
        cout << "Error";
        return -1;
    }

    for (int i = 0; i < number_of_senders; ++i) {
        wstring command_line = L"Sender.exe " + file_name + L" " + to_wstring(number_of_records) + L" " + to_wstring(i);

        bp::child sender(command_line, process_group, bp::std_out > bp::null);
        senders.push_back(move(sender));
    }

    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](const boost::system::error_code&, int) {
        process_group.terminate();
    });

    int action = 1;

    while (true) {
        cout << "1 to read message\n";
        cout << "0 to exit\n";
        cin >> action;
        if (action != 0 && action != 1) {
            cout << "Unknown command";
            continue;
        }
        if (action == 0) {
            break;
        }
        WaitForSingleObject(read_sem, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        file.open(file_name, ios::binary | ios::in);

        // Read message from the file as a string
        string mes;
        getline(file, mes);

        cout << "New message:" << mes << "\n";
        vector<string> file_text;
        string line;
        while (getline(file, line)) {
            file_text.push_back(line);
        }
        file.close();
        file.open(file_name, ios::binary | ios::out);

        // Write updated data back to the file
        for (int i = 0; i < file_text.size(); ++i) {
            file << file_text[i] << "\n";
        }
        file.close();
        ReleaseMutex(mutex);
        ReleaseSemaphore(write_sem, 1, NULL);
    }

    process_group.terminate();
    process_group.wait();
    
    CloseHandle(mutex);
    CloseHandle(read_sem);
    CloseHandle(write_sem);
    return 0;
}
