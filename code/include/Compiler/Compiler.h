#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "LexicalAnalysis.h"
#include "GrammarAnalysis.h"
#include "SemanticAnalysis.h"
#include "IntermediateCodeGeneration.h"
#include "CodeOptimization.h"
#include "CodeGeneration.h"

namespace compiler
{

class Compiler {
public:
	using ptr = std::shared_ptr<Compiler>;
	
	Compiler();
	~Compiler();

	long Compile(const _STD string& src_file, const _STD string& dst_file);

private:
	LexicalPlayer::ptr m_lexical;
	GrammarPlayer::ptr m_grammar;
	SemanticPlayer::ptr m_semantic;
	IntermediateCodePlayer::ptr m_intermediate_code;
	CodeOptimizationPlayer::ptr m_code_optimization;
	CodeGenerationPlayer::ptr m_code_generation;

}; // class Compiler


} // namespace compiler