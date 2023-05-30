#pragma once

#include "Structures.h"
#include "CompilerProcess.h"


namespace compiler
{
	class GrammarAnalysis;

	using GrammarBase = CompilerObjBase<Token, GrammarTree>;
	using GrammarPlayer = CompilePlayer<GrammarAnalysis>;

	class GrammarAnalysis : public GrammarBase
	{
	public:
		GrammarAnalysis() {}
		~GrammarAnalysis() {}

	public:
		virtual long Generate(const input_t& input, output_t& output);

	};// class LexicalAnalysis


} // namespace compiler 