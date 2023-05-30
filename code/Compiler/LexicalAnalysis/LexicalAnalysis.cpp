
#include "Compiler/LexicalAnalysis.h"
#include "File.h"

#include <map>
#include <string>

namespace compiler
{

const static std::map<std::string, TokenType> g_signified_symbols = {
	{"!=", TokenType::SYMBOL}
	, {"==", TokenType::   SYMBOL}
};

void StatusMachine(int stastus, char* content, size_t offset) {

}

LexicalAnalysis::output_t::data_t extract_token(char* start, size_t size) {
	LexicalAnalysis::output_t::data_t rt;
	
	char first_ch = start[0];
	if (first_ch) {
	
	}

	return rt;
}

long LexicalAnalysis::Generate(const input_t& input, output_t& output) 
{
	long rt = 0;
	logger << "lexical";

	auto& tokens = output.details;
	auto& seg_start = input.segment;

	size_t flow_offset = -1;
	size_t extract_size = input.size;
	while (flow_offset < input.size) {
		flow_offset++;
		extract_size = input.size - flow_offset;
		tokens.push_back(extract_token(seg_start + flow_offset, extract_size));
		auto& debug_token = tokens.back();
	}

	// debug print
	size_t max_size = 0;
	for (const auto& it : tokens) {
		size_t size = it.word.size() + 2;
		max_size = max(size, max_size);
	}

	for (const auto& it : tokens) {
		std::cout << format_str(it.word, max_size) << ": " << it.type << std::endl;
	}

	return rt;
}


}