#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

int main() {
    io_service ioService;

    try {
        local::stream_protocol::endpoint endpoint("\\\\.\\pipe\\pipe_name");
        local::stream_protocol::acceptor acceptor(ioService, endpoint);

        while (true) {
            local::stream_protocol::socket socket(ioService);
            acceptor.accept(socket);

            std::cout << "Client connected." << std::endl;

            while (true) {
                char data[128];
                boost::system::error_code error;

                size_t length = socket.read_some(buffer(data), error);

                if (error == error::eof) {
                    std::cout << "Connection closed by client." << std::endl;
                    break;
                } else if (error) {
                    throw boost::system::system_error(error);
                }

                std::cout.write(data, length);
                std::cout << std::endl;

                socket.write_some(buffer("Server received the data."), error);

                if (error) {
                    throw boost::system::system_error(error);
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
