#pragma once

#include <memory>

#include "Structures.h"
#include "CompilerProcess.h"

namespace compiler 
{
class LexicalAnalysis;

using LexicalBase = CompilerObjBase<ByteFlow, Token>;
using LexicalPlayer = CompilePlayer<LexicalAnalysis>;

class LexicalAnalysis : public LexicalBase
{
public:
	LexicalAnalysis() {}
	~LexicalAnalysis() {}

public:
	virtual long Generate(const input_t& input, output_t& output);

};// class LexicalAnalysis


} // namespace compiler 