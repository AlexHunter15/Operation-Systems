#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

int main() {
    io_service ioService;

    try {
        local::stream_protocol::socket socket(ioService);
        socket.connect(local::stream_protocol::endpoint("\\\\.\\pipe\\pipe_name"));

        std::cout << "Connected to server." << std::endl;

        while (true) {
            std::string message;
            std::cout << "Enter message: ";
            std::getline(std::cin, message);

            boost::system::error_code error;
            write(socket, buffer(message), error);

            if (error) {
                throw boost::system::system_error(error);
            }

            char reply[128];
            size_t reply_length = read(socket, buffer(reply), error);

            if (error == error::eof) {
                std::cout << "Connection closed by server." << std::endl;
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }

            std::cout << "Server replied: ";
            std::cout.write(reply, reply_length);
            std::cout << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
