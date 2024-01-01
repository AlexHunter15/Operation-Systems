#include <iostream>
#include <fstream>
#include <string>
#include <boost/process.hpp>

using namespace std;
namespace bp = boost::process;

int main(int argc, char* argv[]) {
    fstream file;
    int action;
    while (true) {
        cout << "1 to write message\n";
        cout << "0 to exit\n";
        cin >> action;
        if (action != 0 && action != 1) {
            cout << "Unknown command" << "\n";
            continue;
        }
        if (action == 0) {
            break;
        }
        cin.ignore();
        string text;
        cout << "Enter message text:";
        getline(cin, text);

        file.open(argv[1], ios::binary | ios::app);
        if (file.is_open()) {
            size_t textLength = text.length();
            file.write(reinterpret_cast<char*>(&textLength), sizeof(textLength)); // Writing length of the string
            file.write(text.c_str(), textLength); // Writing string data
            file.close();
        } else {
            cout << "Error opening file" << "\n";
        }

        cout << "Written successfully" << "\n";
    }

    return 0;
}
