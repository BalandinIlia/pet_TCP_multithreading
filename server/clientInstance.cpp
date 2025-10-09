#include "thread"
#include "sstream"
#include "mutex"
#include "logger.h"
#include "clientInstance.h"
#include "../shared/messages.h"
#include "../shared/utils.h"
#include "../math_Core/host.h"

void solveCase(short id, number num, SOCKET idSocket, std::mutex* mutSocket)
{
	std::vector<number> aNum = CMathCoreHost::one().get(num);
	if (aNum.empty())
	{
		std::array<char, 3> buf = MS::serializeAnsEmpty(id);
		mutSocket->lock();
		sendAll(idSocket, buf.data(), 3);
		mutSocket->unlock();
	}
	else
	{
		std::vector<char> buf = MS::serializeAnsInf(aNum, id);
		mutSocket->lock();
		sendAll(idSocket, buf.data(), static_cast<int>(buf.size()));
		mutSocket->unlock();
	}
}

class CThreadClient
{
public:
	CThreadClient(SOCKET idSocket) : m_idSocket(idSocket) {}

	void run()
	{
		const std::thread::id idThread = std::this_thread::get_id();
		for (;;)
		{
			char c;
			recvAll(m_idSocket, &c, 1);
			if (MS::decodeType(c) != MS::ETypeMes::eReq)
			{
				std::ostringstream mes;
				mes << "Thread with id " << idThread << " serving a client at socket " << m_idSocket << " faced incorrect message code";
				log(mes.str());
			}
			else
			{
				std::array<char, 10> buf;
				recvAll(m_idSocket, buf.data(), 10);
				std::pair<short, number> req = MS::deserializeRequest(buf);
				
				std::ostringstream mes;
				mes << "Thread with id " << idThread << " serving a client at socket " << m_idSocket << " received a request with id " << req.first << " and body " << req.second;
				
				log(mes.str());
				std::thread t(solveCase, req.first, req.second, m_idSocket, &m_mutSend);
				t.detach();
			}
		}
	}

private:
	SOCKET m_idSocket;

	std::mutex m_mutSend;
};

void serveClient(SOCKET idSocket)
{
	{
		const std::thread::id idThread = std::this_thread::get_id();
		std::ostringstream mes;
		mes << "Thread with id " << idThread << " started serving a client at socket " << idSocket;
		log(mes.str());
	}

	CThreadClient thr(idSocket);
	thr.run();
}