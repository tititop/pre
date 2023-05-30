#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "adapt_glog.h"

#ifndef logger
#define logger MXT_INFO_LOGGER
#endif // logger

namespace compiler 
{

enum TokenType {
	PARAM_NAME			// abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890
	, PARAM_TYPE		// size_t, int8_t, int16_t, int32_t, uint32_t, void, 
	, OPERATOR			// () [] < > = != + - * / % & && | || ! # ~
	, KEY_WORLD			// struct static namespace template
	, SYMBOL			// {} , : ; ? ::
};

struct ByteFlow {
	ByteFlow(char* seg = nullptr, size_t len = 0) 
	{
		segment = seg;
		size = len;
	}

	char* segment = nullptr;
	size_t size = 0;
};

struct TokenItem {
	TokenType   type;
	std::string word;
};

struct Token {
	using data_t = TokenItem;

	std::vector<data_t> details;
};

struct GrammarTree {};
struct IntermediateCode {};
struct TargetCode {};

// using SymbolTableBase = CompilerObjBase<ByteFlow, GrammarTree>;
// using CreateSubPassBase = CompilerObjBase<ByteFlow, GrammarTree>;
// using CompilerBuildToolBase = CompilerObjBase<ByteFlow, GrammarTree>;

} // namespace compiler