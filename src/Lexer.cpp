#include <cctype>

#include "fmt/core.h"
#include "include/Lexer/Lexer.hpp"

namespace Lunasm {

  Lexer::Lexer(const std::string& source_code)
    : m_source_code(source_code)
  {}

  char Lexer::eat()
  {
    return m_source_code.at(m_index++);
  }

  void Lexer::step(void)
  {
    if (m_index < m_source_code.length())
      m_index++;
  }

  char Lexer::current_char(void) const
  {
    return m_source_code.at(m_index);
  }

  std::optional<char> Lexer::peek(std::size_t pos)
  {
    if ((m_index + pos) < m_source_code.length())
      return m_source_code.at(m_index);

    return {};
  }

  void Lexer::Tokenizer(void)
  {
    while (m_index < m_source_code.length())
    {
      if (std::isalpha(current_char()))
      {
        std::size_t start = m_index; 
        step(); 
        
        while (std::isalnum(current_char()))      
          step();
       
        std::size_t end = m_index;
        std::string text = m_source_code.substr(start, end);

        fmt::print("Token: {}\n", text);
      }
    }
  }
}
