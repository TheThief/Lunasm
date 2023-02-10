#include <string>
#include <optional>

#include <cctype>
#include <cstdlib>
#include <cstddef>

#include "fmt/core.h"

namespace Lunasm {

  class Lexer 
  {
  public:
    Lexer(const std::string& source_code)
      : m_sourceCode(source_code)
    {}
    
    char current() const
    {
      if (m_index >= m_sourceCode.length())
        return '\0';

      return m_sourceCode.at(m_index);
    }

    void advance()
    {
      m_index++;
    }

    char eat()
    {
      return m_sourceCode.at(m_index++);
    }

    std::optional<char> peek(std::size_t pos)
    {
      if (m_index + pos < m_sourceCode.size())
        return m_sourceCode.at(m_index + pos);

      return {};
    }

    void tokenizer() 
    {
      while (m_index < m_sourceCode.length())
      {
        if (std::isdigit(current()))
        {
          std::size_t start = m_index;

          advance();

          while (std::isdigit(current()) || current() == '.') 
            advance();

          std::size_t end = m_index - start;
          std::string text = m_sourceCode.substr(start, end);

          fmt::print("Token: {}\n", std::stof(text.c_str()));
        }

        if (current() == ' ')
          advance();
        }
    }

  private:
    std::size_t m_index{0};
    std::string m_sourceCode{0};
  };
}

int main()
{
  std::string code = "12.345";

  auto l = Lunasm::Lexer(code);
  l.tokenizer();

  return EXIT_SUCCESS;
}
