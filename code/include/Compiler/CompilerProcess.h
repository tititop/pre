#pragma once

#include "adapt_glog.h"

namespace compiler
{

template<typename _TInput, typename _TOutput>
class CompilerObjBase {
public:
	using ptr = std::shared_ptr<CompilerObjBase>;

	using input_t = _TInput;
	using output_t = _TOutput;

	CompilerObjBase() {};
	~CompilerObjBase() {};

public:
	virtual long Generate(const input_t& input, output_t& output) = 0;

}; // class CompilerObjBase


template<typename _TPlayer>
class CompilePlayer {
public:
	using player_t = _TPlayer;
	using input_t = typename player_t::input_t;
	using output_t = typename player_t::output_t;

	using ptr = std::shared_ptr<CompilePlayer<player_t>>;

	CompilePlayer() {
		m_player = std::shared_ptr<player_t>(new player_t);
	}
	~CompilePlayer() {}

	long Generate(const input_t& input, output_t& output)
	{
		logger << "\n\n";

		logger << "\n" << getPlayerInfo() << " { \n";
		long rt = m_player->Generate(input, output);
		logger << "\n}\n\n\n";
		return rt;
	}

protected:

private:
	std::string getPlayerInfo() const {
		const std::string player_name = typeid(player_t).name();
		const std::string input_name = typeid(input_t).name();
		const std::string output_name = typeid(output_t).name();

		return player_name
			+ "<"
			+ input_name + ", "
			+ output_name
			+ ">";
	}

private:
	std::shared_ptr<player_t> m_player;

}; // class CompilePlayerBase

} // namespace compiler