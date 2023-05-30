

#pragma once

#include <memory>

#include "Structures.h"
#include "CompilerProcess.h"

namespace compiler
{
	class SemanticAnalysis;

	using SemanticBase = CompilerObjBase<GrammarTree, GrammarTree>;
	using SemanticPlayer = CompilePlayer<SemanticAnalysis>;

	class SemanticAnalysis : public SemanticBase
	{
	public:
		SemanticAnalysis() {}
		~SemanticAnalysis() {}

	public:
		virtual long Generate(const input_t& input, output_t& output);

	};// class SemanticAnalysis


} // namespace compiler 