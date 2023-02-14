#include <cctype>
#include <string_view>
#include <unordered_map>

#include "fmt/core.h"

#include "Lexer.hpp"

#include <fmt/ostream.h>
#include <iomanip>

const std::unordered_map<std::string_view, std::string> Keywords = {

  // Instructions
  {"ldi", "instruction"},
  {"cmp", "instruction"},
  {"jne", "instruction"},

  // Registers
  {"r0", "register"},
  {"r1", "register"},
};

//              current_char, TokenKind
std::unordered_map<char, std::string> LiteralTokens = {
  {'[', "]"},
  {']', "["},
  {'+', "+"},
  {'-', "-"},
  {',', ","},
  {':', ":"},
};

namespace Lunasm {

  Lexer::Lexer(const std::string& source_code)
    : m_source_code(source_code)
  {}

  char Lexer::eat(void)
  {
    return m_source_code.at(m_index++);
  }

  void Lexer::step(void)
  {
    if (m_index < m_source_code.length())
      m_index++;
  }

  void Lexer::skip(std::size_t n = 1)
  {
    m_index += n;
  }

  bool Lexer::is_empty(void) const 
  {
    return m_index >= m_source_code.length();
  }

  char Lexer::current_char(void) const
  {
    return m_source_code.at(m_index);
  }

  std::optional<char> Lexer::peek(std::size_t pos = 1)
  {
    auto index = m_index + pos; 

    if (index < m_source_code.length())
      return m_source_code.at(index);

    return {};
  }

  void Lexer::Tokenizer(void)
  {
    while (m_index < m_source_code.length())
    {
      if (std::isalpha(current_char()))
      {
        std::size_t start = m_index; 
        
        while (!is_empty() && std::isalnum(current_char()))
          step();

        std::size_t end = m_index - start;
        std::string_view text(m_source_code.c_str() + start, end);
        
        std::string kind;

        // is_keyword(current_char())
        // is_register(current_char())
        if (Keywords.find(text) != Keywords.end())
          kind = Keywords.at(text);
        else
        {
          text = "ERR";
          kind = "invalid";
        }
        
        fmt::print("Token( T: \"{:10}\" Kind: <{}>, Line: {}, Offset: {:02d} )\n", std::quoted(text), kind, m_line, m_index);
      }

      else if (std::isspace(current_char()))
      {
        if (current_char() == '\n')
          m_line++;

        skip();
      }
      
      else if (current_char() == '$')
      {
        if (!std::isxdigit(peek().value()))
          fmt::print("Exception missing immediate after $");

        skip(); // Skipping '$' character
        std::size_t start = m_index;

        while (!is_empty() && std::isxdigit(current_char()))
          step();

        std::size_t end = m_index - start;
        std::string_view text(m_source_code.c_str() + start, end);

        fmt::print("Token( T: \"{}\"\t Kind: <{}>,\t Line: {},\t Offset: {:02d} )\n", text, "immediate", m_line, m_index);
      }

      else if (LiteralTokens.find(current_char()) != LiteralTokens.end())
      {
        auto text = LiteralTokens.at(current_char());
        fmt::print("Token( T: \"{}\"\t Kind: <{}>,\t Line: {},\t Offset: {:02d} )\n", text, "punctuation", m_line, m_index);
        step();
      }

      else 
        step();    
    }
    
    fmt::print("Token( T: \"{}\"\t Kind: <{}>,\t Line: {},\t Offset: {:02d} )\n", "EOF", "punctuation", m_line, m_index);
  }
}
