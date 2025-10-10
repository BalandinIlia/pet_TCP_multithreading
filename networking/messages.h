#pragma once
#include "pch.h"
#include "../controls/controls.h"
/// <summary>
/// This file introduces my own protocol for client-server communication working over TCP.
///
/// There are 3 types of messages:
/// 1. Request: a request from the client to the server
/// 2. Answer No: an answer from the server to the client that decomposition is impossible
/// 3. Answer Yes: an answer from the server to the client that decomposition is possible.
///	               This message also contains decomposition elements.
/// 
/// Each message is serialized into binary format. The structure is the following:
/// First byte: always encodes message type. The following structure depends on message type.
/// 1. Request:
///		2nd-3rd bytes: request id in uint16_t format
///		4th-11th bytes: request body (the number for decomposition) in uint64_t format
/// 2. Answer No:
///		2nd-3rd bytes: request id in uint16_t format
/// 3. Answer Yes:
///		2nd byte: quantity of decomposition components in uint8_t format
///		3rd-4th bytes: request id in uint16_t format
///		other 8*(quantity of components) bytes: array of quantity components in uint64_t format
/// </summary>

namespace MS
{
	// message types
	enum class ETypeMes
	{
		// client request with particular number
		eReq,
		// server answer that there is no sum
		eAnsNo,
		// server answer with components of sum
		eAnsYes,
		// special incorrect case
		eError
	};

	char codeType(ETypeMes t);

	ETypeMes decodeType(char c);

	int length(ETypeMes t);

	std::array<char, 11> serializeRequest(number req, short idRequest);

	std::vector<char> serializeAnsYes(const std::vector<number> aNum, short id);

	std::array<char, 3> serializeAnsNo(short id);

	std::pair<short, number> deserializeRequest(const std::array<char, 10>& rawData);

	int bufSizeAnsYes(char c);

	std::pair<short, std::vector<number>> deserializeAnsYes(const std::vector<char>& rawData);

	short deserializeAnsNo(const std::array<char, 2>& rawData);
}