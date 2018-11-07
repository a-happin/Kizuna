#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <kizuna/omajinai.hpp>
#include <kizuna/parser.hpp>
#include <kizuna/ast.hpp>
#include <kizuna/except.hpp>
#include <kizuna/utility.hpp>

auto main () -> int
{
  kizuna::omajinai ();

  auto source = kizuna::file_to_string (std::cin);

  auto parser = kizuna::parser {source};

  try
  {
    auto c = parser.parse_int_literal ();
    std::cout << "Parse succeeded: " << c.token << std::endl;
  }
  catch (const char * error)
  {
    std::cout << error << std::endl;
  }
  std::cout << "Rest: " << parser.get_rest () << std::flush;
}

