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

    // This is an experimental binding of the socket.
    // Here I don't use standard structure to prepare binary
    // data for bind function. Instead I prepare binary data
    // manually, operating separate bytes.
    //
    // In order to do this, I studied how sockaddr_in looks in
    // binary format for my case:
    // 1. 2 bytes of sin_family field in unsigned short format
    // 2. 2 bytes of sin_port field in unisigned short format
    // 5. 4 bytes of S_addr in unsigned long format
    // 6. 8 unused bytes
    // total 16 bytes
    
    // I use long long for data alignment. bf is just a 16-byte
    // buffer with appropriate alignment.
    std::array<long long, 2> bf;
    // pointer to the buffer. I need char* to be able to operate
    // individual bytes
    char* buf = reinterpret_cast<char*>(&bf);
    
    // fill sin_family
    // prepare sin_family in correct format (unsigned short)
    const unsigned short sf = AF_INET;
    // pointer to the first byte of sin_family value
    const char* psf = reinterpret_cast<const char*>(&sf);
    // copy bytes
    buf[0] = psf[0];
    buf[1] = psf[1];
    
    // fill s_addr
    // prepare s_addr in correct format (unsigned long)
    const unsigned long sa = addrIP;
    // pointer to the first byte of s_addr value
    const char* psa = reinterpret_cast<const char*>(&sa);
    // copy bytes in reverse order to swap endianness
    // s_addr (in the structure) starts from byte indexed 4
    // so here I fill bytes number 4, 5, 6, 7
    buf[4] = psa[3];
    buf[5] = psa[2];
    buf[6] = psa[1];
    buf[7] = psa[0];
    
    // fill sin_port (analogous to 2 previous steps)
    const unsigned short sp = port;
    const char* psp = reinterpret_cast<const char*>(&sp);
    buf[2] = psp[1];
    buf[3] = psp[0];
    
    bind(idSocket, reinterpret_cast<sockaddr*>(buf), 16);

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
