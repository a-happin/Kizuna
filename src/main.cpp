#include <iostream>
#include <string>
#include <utility> // std::move
#include <cstdint> // std::int64_t

#include <kizuna/parser.hpp>


inline auto parse (kizuna::input_stream & s)
{
  auto res = kizuna::parser::parse_literal (s);
  // kizuna::parse_eof (s);
  return res;
}

auto main () -> int
{
  // omajinai
  std::cin.tie (nullptr);
  std::ios_base::sync_with_stdio (false);

  auto s = kizuna::input_stream (std::cin , "stdin");
  // auto s = kizuna::input_stream ("");


  try
  {
    auto res = parse (s);
    std::cout << res << std::endl;
    std::cout << "Rest: " << s.rest () << std::flush;
  }
  catch (kizuna::exception & e)
  {
    std::cerr << e.what () << std::endl;
  }



}
