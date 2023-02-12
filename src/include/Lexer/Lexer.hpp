#pragma once

#include <string>
#include <optional>

namespace Lunasm {
	class Lexer
	{
	public:
		Lexer(const std::string& source_code);
	public:
		char eat();
		void step(void);
		char current_char(void) const;
		std::optional<char> peek(std::size_t pos);
	public:
		void Tokenizer();
	private:
		std::size_t m_index{0};
		std::string m_source_code{0};
	};
};
