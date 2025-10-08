#pragma once
#include "array"
#include "vector"
#include "params.h"

namespace MS
{

	// message types
	enum class ETypeMes
	{
		// client request with particular number
		eReq,
		// server answer that there is no sum
		eAnsEmpty,
		// server answer with components of sum
		eAnsInf,
		// special incorrect case
		eError
	};

	char codeType(ETypeMes t)
	{
		switch (t)
		{
		case ETypeMes::eReq: return 'a';
		case ETypeMes::eAnsEmpty: return 'b';
		case ETypeMes::eAnsInf: return 'c';
		case ETypeMes::eError: return 0;
		}
	}

	ETypeMes decodeType(char c)
	{
		switch (c)
		{
		case 'a': return ETypeMes::eReq;
		case 'b': return ETypeMes::eAnsEmpty;
		case 'c': return ETypeMes::eAnsInf;
		default: return ETypeMes::eError;
		}
	}

	int length(ETypeMes t)
	{
		switch (t)
		{
		case ETypeMes::eReq: return 10;
		case ETypeMes::eAnsEmpty: return 2;
		case ETypeMes::eAnsInf: return -1;
		case ETypeMes::eError: return -2;
		}
	}

	std::array<char, 11> serializeRequest(number req, short idRequest)
	{
		char* pCode = nullptr;
		uint16_t* pId = nullptr;
		uint64_t* pNum = nullptr;

		std::array<uint64_t, 2> buf;
		char* pBuf = reinterpret_cast<char*>(buf.data()) + 5;
		pCode = pBuf;
		pId = reinterpret_cast<uint16_t*>(pBuf + 1);
		pNum = reinterpret_cast<uint64_t*>(pBuf + 3);

		*pCode = codeType(ETypeMes::eReq);
		*pId = static_cast<uint16_t>(idRequest);

		std::array<char, 11> ans;
		for (int i = 0; i < 11; i++)
			ans[i] = p[i];
		return ans;
	}

	std::pair<short, number> deserializeRequest(const std::array<char, 10>& rawData)
	{
		uint16_t* pId = nullptr;
		uint64_t* pNum = nullptr;

		std::array<uint64_t, 2> buf;
		char* p = reinterpret_cast<char*>(buf.data());
		p += 6;
		for (int i = 0; i < 10; i++)
			p[i] = rawData[i];

		pId = reinterpret_cast<uint16_t*>(p);
		pNum = reinterpret_cast<uint64_t*>(p + 2);

		short id = static_cast<short>(*pId);
		short num = static_cast<number>(*pNum);

		return std::pair<short, number>(id, num);
	}

	std::vector<char> serializeAnsInf(const std::vector<number> aNum, short id)
	{
		// quantity of elements
		const int N = static_cast<int>(aNum.size());

		// pointer to socket buffer
		char* pBuf = nullptr;
		// length of socket buffer
		int lenBuf = -1;

		// pointer to answer code in the buffer
		char* pCode = nullptr;
		// pointer to quantity of numbers in the buffer
		uint8_t* pQuant = nullptr;
		// pointer to id
		uint16_t* pId = nullptr;
		// pointer of array of numbers in the buffer
		uint64_t* aVal = nullptr;

		// Here we create a structure which will contain buffer and link it to abovementioned variables.
		std::vector<uint64_t> bufInternal;
		bufInternal.resize(N + 1);
		pBuf = reinterpret_cast<char*>(bufInternal.data()) + 4;
		lenBuf = N * 8 + 4;
		pCode = pBuf;
		pQuant = reinterpret_cast<uint8_t*>(pBuf) + 1;
		pId = reinterpret_cast<uint16_t*>(pBuf + 2);
		aVal = bufInternal.data() + 1;

		// fill the buffer
		*pCode = codeType(ETypeMes::eAnsInf);
		*pQuant = static_cast<uint8_t>(N);
		*pId = static_cast<uint16_t>(id);
		for (int i = 0; i < N; i++)
			aVal[i] = aNum[i];

		// return the buffer
		std::vector<char> ans;
		ans.reserve(lenBuf);
		for (int i = 0; i < lenBuf; i++)
			ans[i] = pBuf[i];

		return ans;
	}

	int bufSizeAnsInf(char c)
	{
		uint8_t* p = reinterpret_cast<uint8_t*>(c);
		return (*p) * 8 + 2;
	}

	std::vector<number> deserializeAnsInf(short& id, const std::vector<char>& aCh)
	{
		const int N = static_cast<int>(aCh.size() - 2) / 8;

		std::vector<uint64_t> buf;
		buf.reserve(N + 1);
		
		char* p = reinterpret_cast<char*>(buf.data());
		p += 6;
		for (int i = 0; i < aCh.size(); i++)
			p[i] = aCh[i];

		id = *(reinterpret_cast<uint16_t*>(p));

		std::vector<number> ans;
		for (int i = 0; i < N; i++)
			ans.push_back(buf[i]);
		return ans;
	}
}
