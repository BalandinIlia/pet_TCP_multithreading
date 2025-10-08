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
		default: ETypeMes::eError;
		}
	}

	std::array<char, 9> serializeRequest(number req)
	{
		number buf[2] = {0, 0};
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
		number ans = 0;
		number* pN = &ans;
		char* p = reinterpret_cast<char*>(pN);
		for (int i = 0; i < 8; i++)
			p[i] = rawData[i];
		return ans;
	}
}