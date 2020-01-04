#ifndef KIZUNA_PARSER_PARSER_HPP
#define KIZUNA_PARSER_PARSER_HPP

#include <sstream>

#include <kizuna/parser/source.hpp>
#include <kizuna/parser/exception.hpp>
#include <kizuna/optional.hpp>

namespace kizuna::parser
{
  // --------------------------------
  // 文字判別
  // --------------------------------
  inline constexpr auto is_eof (const source_iterator & ite) noexcept
  {
    return ite.is_eof ();
  }
  inline constexpr auto is_digit (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && '0' <= * ite && * ite <= '9';
  }
  inline constexpr auto is_upper (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && 'A' <= * ite && * ite <= 'Z';
  }
  inline constexpr auto is_lower (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && 'a' <= * ite && * ite <= 'z';
  }
  inline constexpr auto is_alpha (const source_iterator & ite) noexcept
  {
    return is_upper (ite) || is_lower (ite);
  }
  inline constexpr auto is_letter_head (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && (is_alpha (ite) || * ite == '_');
  }
  inline constexpr auto is_letter_tail (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && (is_letter_head (ite) || is_digit (ite) || * ite == '-');
  }
  inline constexpr auto is_endline (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && (* ite == '\n' || * ite == '\r');
  }
  inline constexpr auto is_space_noendline (const source_iterator & ite) noexcept
  {
    return ! is_eof (ite) && (* ite == ' ' || * ite == '\t');
  }
  inline constexpr auto is_space (const source_iterator & ite) noexcept
  {
    return is_space_noendline (ite) || is_endline (ite);
  }


  // --------------------------------
  // skip
  // --------------------------------
  inline auto skip_spaces (source_iterator & ite) -> void
  {
    while (is_space (ite)) ++ ite;
  }
  inline auto skip_spaces_noendline (source_iterator & ite) -> void
  {
    while (is_space_noendline (ite)) ++ ite;
  }



  // --------------------------------
  // parser generator
  // --------------------------------

  // rollback
  template <typename F>
  inline constexpr auto rollback (F f)
  {
    return [=] (source_iterator & ite)
    {
      auto backup = ite;
      try
      {
        return f (ite);
      }
      catch (const parser_error & e)
      {
        ite = backup;
        throw e;
      }
    };
  }

  // or
  template <typename F>
  inline constexpr auto or_ (F f)
  {
    return [=] (source_iterator & ite)
    {
      auto backup = ite;
      try
      {
        return f (ite);
      }
      catch (const parser_error & e)
      {
        ite = backup;
        throw e;
      }
    };
  }
  template <typename F, typename ... Rest>
  inline constexpr auto or_ (F f, Rest ... rest)
  {
    return [=] (source_iterator & ite)
    {
      auto backup = ite;
      try
      {
        return f (ite);
      }
      catch (const parser_error & e1)
      {
        ite = backup;
        try
        {
          return or_ (rest ...) (ite);
        }
        catch (const parser_error & e2)
        {
          throw e1 | e2;
        }
      }
    };
  }

  // parse_char
  inline constexpr auto parse_char (char c)
  {
    return [=] (source_iterator & ite) -> optional <char> {
      if (is_eof (ite)) return nullopt;
      if (* ite == c) return * ite ++;
      else return nullopt;
    };
  }

  // parse_char (range version)
  inline constexpr auto parse_char (const std::pair <char, char> & range)
  {
    return [=] (source_iterator & ite) -> optional <char>
    {
      if (is_eof (ite)) return nullopt;
      if (range.first <= * ite && * ite <= range.second) return * ite ++;
      else return nullopt;
    };
  }

  // parse_char (or version)
  inline constexpr auto parse_char_or (char c1, char c2)
  {
    return [=] (source_iterator & ite) -> optional <char> {
      if (is_eof (ite)) return nullopt;
      if (* ite == c1 || * ite == c2) return * ite ++;
      else return nullopt;
    };
  }


  // --------------------------------
  // parse
  // --------------------------------
  inline auto parse_anychar (source_iterator & ite) -> char
  {
    if (is_eof (ite))
    {
      throw parser_error {"expected any char, but too short", ite};
    }
    else
    {
      return * ite ++;
    }
  }

  inline constexpr auto parse_unary_minus = parse_char ('-');
  inline constexpr auto parse_0 = parse_char ('0');

  // -?(0|[1-9][0-9]*)
  inline auto parse_decimal_int_literal (source_iterator & ite)
  {
    skip_spaces (ite);
    auto first = ite;

    // -?
    parse_unary_minus (ite);

    // 0
    if (parse_0 (ite))
    {
      if (is_letter_tail (ite))
      {
        throw parser_error {"unknown literal suffix.", ite};
      }
      else
      {
        return std::string {first, ite};
      }
    }

    // [1-9]
    constexpr auto parse_1_to_9 = parse_char ({'1', '9'});
    if (not parse_1_to_9 (ite))
    {
      throw parser_error {"expected digit, but not digit.", ite};
    }

    // [0-9]*
    while (is_digit (ite)) ++ ite;

    // suffix check
    if (is_letter_tail (ite))
    {
      throw parser_error {"unknown literal suffix.", ite};
    }

    return std::string {first, ite};
  }

  // 0[0-7]+
  inline auto parse_octal_int_literal (source_iterator & ite)
  {
    skip_spaces (ite);
    auto first = ite;

    // -?
    parse_unary_minus (ite);

    // 0
    if (not parse_0 (ite))
    {
      throw parser_error {"expected octal digit, but not octal digit.", ite};
    }

    // [0-7]+
    constexpr auto parse_0_7 = parse_char ({'0', '7'});
    if (! parse_0_7 (ite))
    {
      throw parser_error {"expected octal digit, but not octal digit.", ite};
    }
    while (parse_0_7 (ite));

    // suffix check
    if (is_letter_tail (ite))
    {
      throw parser_error {"unknown literal suffix.", ite};
    }

    return std::string {first, ite};
  }

  // -?0[Xx][0-9A-Fa-f]+
  inline auto parse_hexadecimal_int_literal (source_iterator & ite)
  {
    skip_spaces (ite);
    auto first = ite;

    // -?
    parse_unary_minus (ite);

    // 0
    if (not parse_0 (ite))
    {
      throw parser_error {"expected hexadecimal digit, but not hexadecimal digit.", ite};
    }

    // [Xx]
    auto parse_x = parse_char_or ('X', 'x');
    if (not parse_x (ite))
    {
      throw parser_error {"expected hexadecimal digit, but not hexadecimal digit.", ite};
    }

    // [0-9A-Fa-f]+
    auto is_hexadecimal_digit = [] (char x) {
      return ('0' <= x && x <= '9') || ('A' <= x && x <= 'F') || ('a' <= x && x <= 'f');
    };
    if (is_eof (ite) || ! is_hexadecimal_digit (* ite))
    {
      throw parser_error {"expected hexadecimal digit, but not hexadecimal digit.", ite};
    }
    while (! is_eof (ite) && is_hexadecimal_digit (* ite)) ++ ite;

    // suffix check
    if (is_letter_tail (ite))
    {
      throw parser_error {"unknown literal suffix.", ite};
    }

    return std::string {first, ite};
  }

  inline auto parse_int_literal (source_iterator & ite)
  {
    skip_spaces (ite);
    return or_ (parse_decimal_int_literal, parse_octal_int_literal, parse_hexadecimal_int_literal) (ite);
  };

  inline auto parse_keyword (const std::string & word, source_iterator & ite) -> bool
  {
    auto ite_backup = ite;
    for (auto && elem : word)
    {
      if (ite.is_eof ())
      {
        throw parser_error {"parse_keyword: too short", ite};
      }
      if (elem != * ite)
      {
        ite = ite_backup;
        throw parser_error {"parse_keyword: expected keyword ", ite};
      }
      ++ ite;
    }
    return true;
  }
} // namespace kizuna::parser

#endif // KIZUNA_PARSER_PARSER_HPP
