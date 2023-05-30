#pragma once

#include "Structures.h"
#include "CompilerProcess.h"


namespace compiler
{

class CodeOptimization;

using CodeOptimizationBase = CompilerObjBase<IntermediateCode, IntermediateCode>;
using CodeOptimizationPlayer = CompilePlayer<CodeOptimization>;

class CodeOptimization : public CodeOptimizationBase
{
public:
	CodeOptimization() {}
	~CodeOptimization() {}

public:
	virtual long Generate(const input_t& input, output_t& output);

};// class LexicalAnalysis


} // namespace compiler 