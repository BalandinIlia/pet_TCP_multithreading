#pragma once
#include "pch.h"
#include "../controls/controls.h"

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

	char codeType(ETypeMes t);

	ETypeMes decodeType(char c);

	int length(ETypeMes t);

	std::array<char, 11> serializeRequest(number req, short idRequest);

	std::vector<char> serializeAnsInf(const std::vector<number> aNum, short id);

	std::array<char, 3> serializeAnsEmpty(short id);

	std::pair<short, number> deserializeRequest(const std::array<char, 10>& rawData);

	int bufSizeAnsInf(char c);

	std::pair<short, std::vector<number>> deserializeAnsInf(const std::vector<char>& rawData);

	short deserializeAnsEmpty(const std::array<char, 2>& rawData);
}