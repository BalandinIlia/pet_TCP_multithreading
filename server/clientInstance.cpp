#include "thread"
#include "sstream"
#include "mutex"
#include "logger.h"
#include "clientInstance.h"
#include "../networking/messages.h"
#include "../networking/utils.h"
#include "../math_Core/host.h"

void solveCase(short id, number num, SOCKET idSocket, std::mutex* mutSocket, int idClient)
{
	std::vector<number> aNum = CMathCoreHost::one().get(num);
	bool bSent = true;
	if (aNum.empty())
	{
		std::array<char, 3> buf = MS::serializeAnsNo(id);
		mutSocket->lock();
		bSent = sendAll(idSocket, buf.data(), 3);
		mutSocket->unlock();
	}
	else
	{
		std::vector<char> buf = MS::serializeAnsYes(aNum, id);
		mutSocket->lock();
		bSent = sendAll(idSocket, buf.data(), static_cast<int>(buf.size()));
		mutSocket->unlock();
	}
	if(!bSent)
	{
		std::ostringstream mes;
		mes << "Unable to sent an answer to client " << idClient << " for request with id " << id << " due to network error";
		log(mes.str());
	}
}

class CThreadClient
{
public:
	CThreadClient(SOCKET idSocket, int idClient) : 
		m_idSocket(idSocket),
		m_idClient(idClient)
	{}

	void run()
	{
		std::vector<std::thread> aThrChild;
		for (;;)
		{
			char c;
			if (!recvAll(m_idSocket, &c, 1))
			{
				std::ostringstream mes;
				mes << "Session with client " << m_idClient << " is closed due to network error";
				log(mes.str());
				break;
			}
			if (MS::decodeType(c) != MS::ETypeMes::eReq)
			{
				std::ostringstream mes;
				mes << "Client " << m_idClient << " sent a message with incorrect code";
				log(mes.str());
				break;
			}
			else
			{
				std::array<char, 10> buf;
				if(!recvAll(m_idSocket, buf.data(), 10))
				{
					std::ostringstream mes;
					mes << "Session with client " << m_idClient << " is closed due to network error";
					log(mes.str());
					break;
				}

				std::pair<short, number> req = MS::deserializeRequest(buf);
				
				std::ostringstream mes;
				mes << "Client " << m_idClient << " sent a request with id " << req.first << " and number " << req.second;
				log(mes.str());

				std::thread t(solveCase, req.first, req.second, m_idSocket, &m_mutSend, m_idClient);
				aThrChild.push_back(std::move(t));
			}
		}
		for (std::thread& t : aThrChild)
			t.join();
	}

private:
	SOCKET m_idSocket;

	int m_idClient;

	std::mutex m_mutSend;
};

void serveClient(SOCKET idSocket, int idClient)
{
	{
		const std::thread::id idThread = std::this_thread::get_id();
		std::ostringstream mes;
		mes << "Thread with id " << idThread << " started serving a client " << idClient << " at socket " << idSocket;
		log(mes.str());
	}

	CThreadClient thr(idSocket, idClient);
	thr.run();
}