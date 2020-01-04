#ifndef KIZUNA_PARSER_HPP
#define KIZUNA_PARSER_HPP

#include <string>
#include <stdexcept>

#include <kizuna/parser/parser.hpp>

namespace kizuna
{
  inline auto parse (std::istream & in, const std::string & filename = "(temporary string)")
  {
    using namespace kizuna::parser;
    source_t source (in, filename);
    auto ite = source.begin ();
    try
    {
      auto res = parse_int_literal (ite);
      auto rest = std::string {ite, source.end ()};
      return make_pair (res, rest);
    }
    catch (const parser_error & e)
    {
      throw std::runtime_error {e.what (source)};
    }
  }
} // namespace kizuna

#endif // KIZUNA_PARSER_HPP
