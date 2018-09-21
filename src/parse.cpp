#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <kizuna/omajinai.hpp>
#include <kizuna/packed_iterator.hpp>
#include <kizuna/ast.hpp>
#include <kizuna/except.hpp>
#include <kizuna/utility.hpp>

namespace kizuna
{
  /*
  struct parse_error : std::runtime_error {
    explicit parse_error (const std::string & msg)
      : std::runtime_error (msg)
    {}
  };


  struct parse_eof_error : std::runtime_error {
    explicit parse_eof_error (const std::string & msg)
      : std::runtime_error (msg)
    {}
  };
  */


  auto parse_anychar (packed_iterator & p);
  auto parse_anychar (packed_iterator & p)
  {
    return p.get ();
  }


  auto parse_char (char c , packed_iterator & p);
  auto parse_char (char c , packed_iterator & p)
  {
    if (p.is_char (c))
    {
      return p.get ();
    }
    else
    {
      throw "Parse failed: not match in parse_char";
    }
  }


  auto parse_int_literal (packed_iterator & p);
  auto parse_int_literal (packed_iterator & p)
  {
    p.skip_space ();
    auto backup = p;
    while (p.is_digit ())
    {
      p.next ();
    }
    if (backup.ite == p.ite)
    {
      throw "Parse failed: not int_literal";
    }
    return ast::ast_base {std::string {backup.ite , p.ite} , std::string {"int"} , {}};
  }


} // namespace kizuna

auto main () -> int
{
  kizuna::omajinai ();

  auto source = kizuna::get_source (std::cin);

  auto p = kizuna::make_packed_iterator (source);

  try
  {
    auto c = kizuna::parse_int_literal (p);
    std::cout << "Parse succeeded: " << c.token << std::endl;
  }
  catch (const char * error)
  {
    std::cout << error << std::endl;
  }
  std::cout << "Rest: " << p.show () << std::flush;
}

