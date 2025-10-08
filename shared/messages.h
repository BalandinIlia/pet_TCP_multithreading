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

	std::array<char, 9> serializeRequest(number req)
	{
		uint64_t buf[2] = {0, 0};
		number* pN = buf;
		char* p = reinterpret_cast<char*>(pN);
		p += 7;
		*p = codeType(ETypeMes::eReq);
		buf[1] = req;

		std::array<char, 9> ans;
		for (int i = 0; i < 9; i++)
			ans[i] = p[i];
		return ans;
	}

	number deserializeRequest(const std::array<char, 8>& rawData)
	{
		uint64_t ans = 0;
		uint64_t* pN = &ans;
		char* p = reinterpret_cast<char*>(pN);
		for (int i = 0; i < 8; i++)
			p[i] = rawData[i];
		return ans;
	}

	std::vector<char> serializeAnsInf(const std::vector<number> aNum)
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
		// pointer of array of numbers in the buffer
		uint64_t* aVal = nullptr;

		// Here we create a structure which will contain buffer and link it to abovementioned variables.
		std::vector<uint64_t> bufInternal;
		bufInternal.resize(N + 1);
		pBuf = reinterpret_cast<char*>(bufInternal.data()) + 6;
		lenBuf = N * 8 + 2;
		pCode = pBuf;
		pQuant = reinterpret_cast<uint8_t*>(pBuf) + 1;
		aVal = bufInternal.data() + 1;

		// fill the buffer
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
		return *p;
	}

	std::vector<number> deserializeAnsInf(const std::vector<char>& aCh)
	{
		const int N = static_cast<int>(aCh.size()) / 8;
		std::vector<number> ans;
		ans.reserve(N);
		const uint64_t* p = reinterpret_cast<const uint64_t*>(aCh.data());
		for (int i = 0; i < N; i++)
			ans.push_back(p[i]);
		return ans;
	}
}
