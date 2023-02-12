#include <string>
#include <optional>

#include <cctype>
#include <cstdlib>
#include <cstddef>

#include "fmt/core.h"
#include "cxxopts.hpp"

#include "include/Lexer/Lexer.hpp"

int main(int argc, char* argv[])
{
  cxxopts::Options options("Lunasm", "Assembly language for the Luna16 fantasy console.");

  options.add_options()
    ("h,help", "Show usage menu") 
  ;

  auto result = options.parse(argc, argv);

  if (result.count("help"))
  {
    fmt::print("{}", options.help());
    std::exit(0);
  }

  auto l = Lunasm::Lexer("ldi r1, $01");
  l.Tokenizer();

  return EXIT_SUCCESS;
}
