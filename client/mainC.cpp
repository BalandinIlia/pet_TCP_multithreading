#include "winsock2.h"
#include "iostream"
#include "thread"
#include "mutex"
#include "../networking/messages.h"
#include "../networking/utils.h"

// mutex controlling console
std::mutex mutCons;

void sendCycle(SOCKET idSocket)
{
    short id = 1;
    for (;;)
    {
        std::cin.get();
        std::cin.get();
        mutCons.lock();
        number num = 0;
        std::cin >> num;
        std::cout << std::endl << "Sent request id " << id << " with number " << num << std::endl;
        mutCons.unlock();
        std::array<char, 11> mes = MS::serializeRequest(num, id);
        sendAll(idSocket, mes.data(), 11);
        id++;
    }
}

void receiveCycle(SOCKET idSocket)
{
    for (;;)
    {
        char c;
        recvAll(idSocket, &c, 1);
        const MS::ETypeMes t = MS::decodeType(c);
        switch (t)
        {
        case MS::ETypeMes::eAnsEmpty:
        {
            std::array<char, 2> buf;
            recvAll(idSocket, buf.data(), 2);
            const short id = MS::deserializeAnsEmpty(buf);
            mutCons.lock();
            std::cout << std::endl << "Request with id " << id << ": decomposition impossible" << std::endl;
            mutCons.unlock();
            break;
        }
        case MS::ETypeMes::eAnsInf:
        {
            recvAll(idSocket, &c, 1);
            const int sz = MS::bufSizeAnsInf(c);
            std::vector<char> buf;
            buf.resize(sz);
            recvAll(idSocket, buf.data(), sz);
            const std::pair<short, std::vector<number>> ans = MS::deserializeAnsInf(buf);
            mutCons.lock();
            std::cout << std::endl << "Request with id " << ans.first << ": decomposition possible:";
            for (number comp : ans.second)
                std::cout << comp << " ";
            std::cout << std::endl;
            mutCons.unlock();
            break;
        }
        default:
        {
            mutCons.lock();
            std::cout << std::endl << "Incorrect code from server" << std::endl;
            mutCons.unlock();
            break;
        }
        }
    }
}

int main()
{
    CWSAConfig conf;

    std::cout << "Client" << std::endl;

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
    connect(idSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

    std::thread tSend(sendCycle, idSocket);
    std::thread tReceive(receiveCycle, idSocket);
    tSend.join();
    tReceive.join();

    return 0;
}