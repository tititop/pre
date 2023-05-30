

#pragma once

#include <memory>

#include "Structures.h"
#include "CompilerProcess.h"

namespace compiler
{

class IntermediateCodeGeneration;

using IntermediateCodeBase = CompilerObjBase<GrammarTree, IntermediateCode>;
using IntermediateCodePlayer = CompilePlayer<IntermediateCodeGeneration>;

class IntermediateCodeGeneration : public IntermediateCodeBase
{
public:
	IntermediateCodeGeneration() {}
	~IntermediateCodeGeneration() {}

public:
	virtual long Generate(const input_t& input, output_t& output);

};// class IntermediateCodeGeneration


} // namespace compiler 