#include <cctype>

#include "fmt/core.h"
#include "include/Lexer/Lexer.hpp"

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
        std::string text = m_source_code.substr(start, end);

        auto token = fmt::format("kind: instruction, line: {} col: {} text: {}", m_line, m_index, text);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if (std::isspace(current_char()))
        skip();
      
      else if (current_char() == ',')
      {
        step();

        auto token = fmt::format("kind: punctuation, line: {} col: {} text: ,", m_line, m_index);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if (current_char() == '[')
      {
        step();

        auto token = fmt::format("kind: punctuation, line: {} col: {} text: [", m_line, m_index);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if (current_char() == ']')
      {
        step();

        auto token = fmt::format("kind: punctuation, line: {} col: {} text: ]", m_line, m_index);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if (current_char() == '+')
      {
        step();

        auto token = fmt::format("kind: operation, line: {} col: {} text: +", m_line, m_index);
        fmt::print("Token {{ {} }}\n", token);
      }

      else if(current_char() == '\n')
      {
        step();
        m_line++;
      }

      else if (current_char() == '$')
      {
        auto next = peek().value();
        
        if (!std::isxdigit(next))
          fmt::print("Exception missing immediate after $");

        skip(); // Skiping '$' character
        std::size_t start = m_index;

        while (!is_empty() && std::isxdigit(current_char()))
          step();

        std::size_t end = m_index - start;
        std::string text = m_source_code.substr(start, end);


        auto token = fmt::format("kind: immediate, line: {} col: {} text: {}", m_line, m_index, text);
        fmt::print("Token {{ {} }}\n", token);
      }

      else 
        step();    
    }
  }
}
