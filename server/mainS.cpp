#include "winsock2.h"
#include "iostream"
#include "../shared/messages.h"
#include "../shared/utils.h"

int main()
{
    CWSAConfig conf;

    std::cout << "Server" << std::endl;

    SOCKET idSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (idSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        std::cin.get();
        return 0;
    }
        
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(addrIP);
    serverAddr.sin_port = htons(TCPPort);
    bind(idSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    listen(idSocket, SOMAXCONN);

    SOCKET conn = accept(idSocket, nullptr, nullptr);

    std::cerr << "Client connected" << std::endl;

    for (;;)
    {
        char c;
        recvAll(conn, &c, 1);
        const MS::ETypeMes t = MS::decodeType(c);
        switch (t)
        {
        case MS::ETypeMes::eReq:
        {
            std::array<char, 10> buf;
            recvAll(conn, buf.data(), 10);
            const std::pair<short, number> res = MS::deserializeRequest(buf);
            const short idd = res.first;
            std::cout << "Received a request with id " << idd << " and number " << res.second << std::endl;
            _Thrd_sleep_for(10000);
            std::array<char, 3> b;
            b = MS::serializeAnsEmpty(idd);
            sendAll(conn, b.data(), 3);
            break;
        }
        default:
            return 0;
        }
    }

    return 0;
}