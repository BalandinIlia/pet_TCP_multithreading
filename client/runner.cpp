#include "runner.h"
#include "sstream"

std::mutex CRunner::m_mutCons;

CRunner::CRunner(SOCKET idS) : m_idSocket(idS), m_id(1) {}

void CRunner::run()
{
    std::thread ts(sendAdapter, this);
    std::thread tr(recvAdapter, this);
    ts.join();
    tr.join();
}

void CRunner::send()
{
    for (;;)
    {
        // 'a' is a signal to receive a number from the user
        char c = 0;
        while (c != 'a')
            c = std::cin.get();

        // Take a number from the user (from console). So here I lock the console mutex to own the console.
        m_mutCons.lock();
        std::cout << "Input a number: ";
        number num = 0;
        std::cin >> num;
        std::array<char, 11> mes = MS::serializeRequest(num, m_id);
        std::cout << std::endl << "Sent request " << num << ". Request id " << m_id << ".";
        m_mutCons.unlock();

        // Modify the table. So here I lock the table mutex to own the table.
        m_mutTable.lock();
        const bool bOk = sendAll(m_idSocket, mes.data(), 11);
        m_table[m_id] = num;
        m_mutTable.unlock();

        if (!bOk)
        {
            m_mutCons.lock();
            std::cout << std::endl << "Connection with server lost";
            m_mutCons.unlock();
            return;
        }
        
        m_id++;
    }
}

void CRunner::receive()
{
    for (;;)
    {
        char c;
        if (!recvAll(m_idSocket, &c, 1))
        {
            logConnectionLost();
            return;
        }
        const MS::ETypeMes t = MS::decodeType(c);
        switch (t)
        {
        case MS::ETypeMes::eAnsNo:
        {
            std::array<char, 2> buf;
            if(!recvAll(m_idSocket, buf.data(), 2))
            {
                logConnectionLost();
                return;
            }
            const short id = MS::deserializeAnsNo(buf);

            // Here I access the table. While I don't modify the table here, I still have to lock the mutex to
            // make sure the table is not modified while I am accessing it. If I don't do this, I can get some 
            // intermediate table state (half-modified).
            std::ostringstream sError;
            m_mutTable.lock();
            number n = 0;
            if (m_table.find(id) != m_table.end())
                n = m_table[id];
            else
                sError << "Error: inner table does not contain request id " << id << " received from the server.";
            m_mutTable.unlock();

            // Show a number from the user (to console). So here I lock the console mutex to own the console.
            m_mutCons.lock();
            if (!sError.str().empty())
                std::cout << std::endl << sError.str();
            std::cout << std::endl << "Decomposition of " << n << " is impossible";
            m_mutCons.unlock();
            break;
        }
        case MS::ETypeMes::eAnsYes:
        {
            if(!recvAll(m_idSocket, &c, 1))
            {
                logConnectionLost();
                return;
            }
            const int sz = MS::bufSizeAnsYes(c);
            std::vector<char> buf;
            buf.resize(sz);
            if(!recvAll(m_idSocket, buf.data(), sz))
            {
                logConnectionLost();
                return;
            }
            const std::pair<short, std::vector<number>> ans = MS::deserializeAnsYes(buf);
            const short id = ans.first;
            const std::vector<number>& aComp = ans.second;

            std::ostringstream sError;
            m_mutTable.lock();
            number n = 0;
            if (m_table.find(id) != m_table.end())
                n = m_table[id];
            else
                sError << "Error: inner table does not contain request id " << id << " received from the server.";
            m_mutTable.unlock();

            m_mutCons.lock();
            if (!sError.str().empty())
                std::cout << std::endl << sError.str();
            std::cout << std::endl << n << " is decomposable:";
            for (const number& comp : aComp)
                std::cout << comp << " ";
            m_mutCons.unlock();
            break;
        }
        default:
        {
            m_mutCons.lock();
            std::cout << std::endl << "Incorrect code from server";
            m_mutCons.unlock();
            break;
        }
        }
    }
}

void CRunner::logConnectionLost()
{
    m_mutCons.lock();
    std::cout << std::endl << "Connection with server lost";
    m_mutCons.unlock();
}