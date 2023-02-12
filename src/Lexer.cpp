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

  void Lexer::skip(std::size_t n = 1)
  {
    m_index += n;
  }

  bool Lexer::is_empty() const 
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
        
        do { 
          step(); 
        } while (!is_empty() && std::isalnum(current_char()));

        std::size_t end = m_index - start;
        std::string text = m_source_code.substr(start, end);

        fmt::print("Token: {}\n", text);
      }

      else if (std::isspace(current_char()))
        skip();
      
      else if (current_char() == ',')
      {
        fmt::print("Token: ,\n");
        step();
      }

      else if (current_char() == '$')
      {
        auto next = peek().value();
        
        if (!std::isdigit(next))
          fmt::print("Exception missing immediate after $");

        skip();
        std::size_t start = m_index;
        skip();

        while (!is_empty() && std::isdigit(current_char()))
          step();

        std::size_t end = m_index - start;
        std::string text = m_source_code.substr(start, end);

        fmt::print("Token: {}\n", text);
      }

      else 
        step();    
    }
  }
}
