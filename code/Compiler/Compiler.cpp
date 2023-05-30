
#include "Compiler/Compiler.h"

#include "File.h"

namespace compiler {

Compiler::Compiler() 
{
	m_lexical = LexicalPlayer::ptr(new LexicalPlayer);
	m_grammar = GrammarPlayer::ptr(new GrammarPlayer);
	m_semantic = SemanticPlayer::ptr(new SemanticPlayer);
	m_intermediate_code = IntermediateCodePlayer::ptr(new IntermediateCodePlayer);
	m_code_optimization = CodeOptimizationPlayer::ptr(new CodeOptimizationPlayer);
	m_code_generation = CodeGenerationPlayer::ptr(new CodeGenerationPlayer);
}

Compiler::~Compiler() {}

long Compiler::Compile(const _STD string& src_file, const _STD string& dst_file)
{
	struct ByteFlow byte_flow;
	struct Token tokens;
	struct GrammarTree grammar_tree;
	struct IntermediateCode intermedia_code;
	struct TargetCode targett_code;

	long rt = 0;
	const std::string source_dir = "E:\\develop\\Comipiler\\";
	const std::string source_code = "main.c";

	char* file_content = nullptr;
	size_t file_size = MFile::Instance().get_file_content(&file_content, source_dir + source_code);

	std::cout << "file_content: " << std::string(file_content, file_size) << std::endl;

	rt = m_lexical->Generate(byte_flow = ByteFlow(file_content, file_size), tokens);
	rt = m_grammar->Generate(tokens, grammar_tree);
	rt = m_semantic->Generate(grammar_tree, grammar_tree);
	rt = m_intermediate_code->Generate(grammar_tree, intermedia_code);
	rt = m_code_optimization->Generate(intermedia_code, intermedia_code);
	rt = m_code_generation->Generate(intermedia_code, targett_code);


	MFile::Instance().drop_file_content(&file_content, file_size);
	return rt;
}

} // namespace compiler 