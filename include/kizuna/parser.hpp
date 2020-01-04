#ifndef KIZUNA_PARSER_HPP
#define KIZUNA_PARSER_HPP

#include <string>
#include <tuple>
#include <stdexcept>

#include <kizuna/parser/parser.hpp>
#include <kizuna/escape_sequence.hpp>

namespace kizuna
{
  namespace parser_helper
  {
    inline auto get_current_line_col (const parser::source_t & source, const parser::source_iterator & ite)
    {
      size_t line = 1, col = 1;
      auto current_line_begin = source.begin ();
      for (auto source_ite = source.begin (); source_ite != source.end () && source_ite != ite;)
      {
        if (* source_ite == '\r')
        {
          ++ line;
          col = 1;
          ++ source_ite;
          if (source_ite != source.end () && source_ite != ite && * source_ite == '\n')
          {
            ++ source_ite;
          }
          current_line_begin = source_ite;
        }
        else if (* source_ite == '\n')
        {
          ++ line;
          col = 1;
          ++ source_ite;
          current_line_begin = source_ite;
        }
        else
        {
          ++ col;
          ++ source_ite;
        }
      }
      auto current_line_end = current_line_begin;
      while (current_line_end != source.end () && ! is_endline (current_line_end)) ++ current_line_end;
      return std::make_tuple (std::string {current_line_begin, current_line_end}, line, col);
    }
  } // namespace parser_helper

  inline auto parse (std::istream & in, const std::string & filename = "(temporary string)")
  {
    using namespace kizuna::parser;
    source_t source (in, filename);
    auto ite = source.begin ();
    try {
      auto res = parse_int_literal (ite);
      auto rest = std::string {ite, source.end ()};
      return make_pair (res, rest);
    }
    catch (const parser::exception & e)
    {
      using escape_sequence::reset;
      using escape_sequence::bold;
      using escape_sequence::red;
      using escape_sequence::yellow;
      //constexpr const char * error_string [] = {"\033[1;32msuggestion: " , "\033[1;33mwarning: " , "\033[1;31merror: "};
      auto [current_line, line, col] = parser_helper::get_current_line_col (source, e.ite);
      std::ostringstream ss;
      ss << bold << source.filename << ":" << line << ":" << col << ": " << (bold | red) << "error" << reset << bold << ": " << e.message << std::endl;
      ss << reset << current_line << std::endl;
      for (decltype (col) i = 1; i < col; ++ i)
      {
        ss << ' ';
      }
      ss << (bold | yellow) << '^' << reset;
      throw std::runtime_error {ss.str ()};
    }
  }
} // namespace kizuna

#endif // KIZUNA_PARSER_HPP
