#include <cctype>
#include <string_view>

#include "fmt/core.h"
#include "include/Lexer/Lexer.hpp"

#include <vector>

const std::vector<std::string> instructions = {
  "ldi", 
};

const std::vector<std::string> registers = {
  "r0", "r1"
};

#include <unordered_map>

std::unordered_map<char, std::string> LiteralTokens = {
  {'[', "Open brackets"},
  {']', "Open brackets"},
  {'+', "Plus sign"},
  {'-', "Minus sign"},
  {',', "Comma"},
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
        for (const auto &inst : instructions)
          if (inst == text)
           kind = "instruction"; 

        for (const auto &reg : registers)
          if (reg == text)
            kind = "register";
        
        auto token = fmt::format("kind: {}, line: {} col: {} text: {}", kind, m_line, m_index, text);
        fmt::print("Token {{ {} }}\n", token);
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

        auto token = fmt::format("kind: immediate, line: {} col: {} text: {}", m_line, m_index, text);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if (LiteralTokens.find(current_char()) != LiteralTokens.end())
      {
        auto text = LiteralTokens.at(current_char());
        step();

        auto token = fmt::format("kind: punctuation, line: {} col: {} text: {}", m_line, m_index, text);
        fmt::print("Token {{ {} }}\n", token);
      }

      else 
        step();    
    }
    
    auto token = fmt::format("kind: eof, line: {} col: {} text: EOF", m_line, m_index);
    fmt::print("Token {{ {} }}\n", token);
  }
}
