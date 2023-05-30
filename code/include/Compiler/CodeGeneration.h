


#pragma once

#include "Structures.h"
#include "CompilerProcess.h"


namespace compiler
{
	class CodeGeneration;

	using CodeGenerationBase = CompilerObjBase<IntermediateCode, TargetCode>;
	using CodeGenerationPlayer = CompilePlayer<CodeGeneration>;

	class CodeGeneration : public CodeGenerationBase
	{
	public:
		CodeGeneration() {}
		~CodeGeneration() {}

	public:
		virtual long Generate(const input_t& input, output_t& output);

	};// class LexicalAnalysis


} // namespace compiler 