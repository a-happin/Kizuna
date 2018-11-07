#ifndef KIZUNA_PARSER_HPP
#define KIZUNA_PARSER_HPP
#include <string>
#include <utility>
#include <kizuna/ast.hpp>


namespace kizuna
{

  struct parser
  {

  private:

    using value_type = std::string::iterator;

    value_type ite;
    value_type end;
    std::int64_t line;
    std::int64_t col;

  public:

    parser (std::string & s) noexcept
      : ite {s.begin ()}
      , end {s.end ()}
      , line (1)
      , col (1)
    {}


    auto is_eof () const noexcept
    {
      return ite == end;
    }


    auto peek () const
    {
      if (is_eof ())
      {
        throw "Error: out_of_range";
      }
      return (* ite);
    }


    auto peek ()
    {
      if (is_eof ())
      {
        throw "Error: out_of_range";
      }
      return (* ite);
    }


    auto get ()
    {
      if (is_eof ())
      {
        throw "Error: out_of_range";
      }
      ++ col;
      return * ite ++;
    }


    auto next ()
    {
      if (is_eof ())
      {
        throw "Error: out_of_range";
      }
      ++ col;
      ++ ite;
    }



    auto is_char (char c) const noexcept
    {
      return ! is_eof () && peek () == c;
    }


    auto is_digit () const noexcept
    {
      return ! is_eof () && '0' <= peek () && peek () <= '9';
    }


    auto is_upper () const noexcept
    {
      return ! is_eof () && 'A' <= peek () && peek () <= 'Z';
    }


    auto is_lower () const noexcept
    {
      return ! is_eof () && 'a' <= peek () && peek () <= 'z';
    }


    auto is_alpha () const noexcept
    {
      return ! is_eof () && (is_upper () || is_lower ());
    }


    auto is_letter () const noexcept
    {
      return ! is_eof () && (is_alpha () || is_char ('_'));
    }


    auto is_letternum () const noexcept
    {
      return ! is_eof () && (is_letter () || is_digit () || is_char ('-'));
    }


    auto is_endline () const noexcept
    {
      return ! is_eof () && (is_char ('\n') || is_char ('\r'));
    }


    auto is_space () const noexcept
    {
      return ! is_eof () && (is_char (' ') || is_char ('\t') || is_endline ());
    }


    auto skip_space () noexcept
    {
      while (is_space ())
      {
        next ();
      }
    }


    auto get_rest () const noexcept
    {
      return std::string {ite , end};
    }


    auto parse_anychar ()
    {
      return get ();
    }


    auto parse_char (char c)
    {
      return is_char (c) ? get () : throw "Parse failed: not match in parse_char";
    }


    auto parse_int_literal ()
    {
      skip_space ();
      auto backup = ite;
      while (is_digit ())
      {
        next ();
      }
      // 変化なし => 数字じゃない
      if (backup == ite)
      {
        throw "Parse failed: not int_literal";
      }
      return ast::ast_base {std::string {backup , ite} , std::string {"int"} , {}};
    }

  };

} // namespace kizuna

#endif

