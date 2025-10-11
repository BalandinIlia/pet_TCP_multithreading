#include "winsock2.h"
#include "thread"
#include "string"
#include "sstream"
#include "../networking/messages.h"
#include "../networking/utils.h"
#include "logger.h"
#include "clientInstance.h"

int main()
{
    CWSAConfig conf;

    log(std::string("Server"));

    SOCKET idSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (idSocket == INVALID_SOCKET)
    {
        std::ostringstream mes;
        mes << "Socket creation failed: " << WSAGetLastError();
        log(mes.str());
        Sleep(100000);
        return 0;
    }

#if (EXPERIMENTAL_ABI == 1)
    // sockaddr_in binary looks like this:
    // 1. 2 bytes of sin_family field in unsigned short format
    // 2. 2 bytes of sin_port field in unisigned short format
    // 5. 4 bytes of S_addr in unsigned long format
    // 6. 8 unused bytes
    // total 16 bytes
    // I use long long for data alignment. bf is just a 16-byte buffer with appropriate alignment.
    std::array<long long, 2> bf;
    char* buf = reinterpret_cast<char*>(&bf);
    // fill sin_family
    const unsigned short sf = AF_INET;
    const char* psf = reinterpret_cast<const char*>(&sf);
    buf[0] = psf[0];
    buf[1] = psf[1];
    // fill s_addr
    const unsigned long sa = addrIP;
    const char* psa = reinterpret_cast<const char*>(&sa);
    buf[4] = psa[3];// swap bytes to change endiannes
    buf[5] = psa[2];
    buf[6] = psa[1];
    buf[7] = psa[0];
    // fill sin_port
    const unsigned short sp = port;
    const char* psp = reinterpret_cast<const char*>(&sp);
    buf[2] = psp[1];
    buf[3] = psp[0];
    bind(idSocket, reinterpret_cast<sockaddr*>(buf), 16);
#else
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(addrIP);
    serverAddr.sin_port = htons(port);
    bind(idSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
#endif

    listen(idSocket, SOMAXCONN);

    // client identificator
    // Receive a connection from a client and serve the client in a separate thread. 
    // This thread keeps listening for new connections. This design allows to serve
    // several clients simultaneously.
    int idClient = 1;
    for (;;)
    {
        SOCKET conn = accept(idSocket, nullptr, nullptr);
        std::thread t(serveClient, conn, idClient);
        idClient++;
        t.detach();
    }

    return 0;
}