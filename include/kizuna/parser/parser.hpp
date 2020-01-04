#ifndef KIZUNA_PARSER_PARSER_HPP
#define KIZUNA_PARSER_PARSER_HPP

#include <sstream>

#include <kizuna/parser/source.hpp>
#include <kizuna/parser/exception.hpp>
#include <kizuna/optional.hpp>

namespace kizuna::parser
{
  // 文字判別
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


  // skip
  inline auto skip_spaces (source_iterator & ite) -> void
  {
    while (is_space (ite)) ++ ite;
  }
  inline auto skip_spaces_noendline (source_iterator & ite) -> void
  {
    while (is_space_noendline (ite)) ++ ite;
  }


  //rollback
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
      catch (const exception & e)
      {
        ite = backup;
        throw e;
      }
    };
  }

  template <typename F, typename G>
  inline constexpr auto or_ (F f, G g)
  {
    return [=] (source_iterator & ite)
    {
      auto backup = ite;
      try
      {
        return f (ite);
      }
      catch (const exception &)
      {
        ite = backup;
      }
      try
      {
        return g (ite);
      }
      catch (const exception & e2)
      {
        ite = backup;
        throw e2;
      }
    };
  }


  // parse
  inline auto parse_anychar (source_iterator & ite) -> char
  {
    if (is_eof (ite))
    {
      throw exception ("too short", ite);
    }
    else
    {
      return * ite ++;
    }
  }

  inline constexpr auto parse_char (char c)
  {
    return [=] (source_iterator & ite) -> optional <char> {
      if (is_eof (ite)) return nullopt;
      if (* ite == c) return * ite ++;
      else return nullopt;
    };
  }

  inline constexpr auto parse_char (const std::pair <char, char> & range)
  {
    return [=] (source_iterator & ite) -> optional <char>
    {
      if (is_eof (ite)) return nullopt;
      if (range.first <= * ite && * ite <= range.second) return * ite ++;
      else return nullopt;
    };
  }

  inline auto parse_digits = rollback ([] (source_iterator & ite)
  {
      skip_spaces (ite);
      std::stringstream ss;
      if (is_digit (ite))
      {
        ss << * ite ++;
      }
      else
      {
        throw exception ("parse_digits: not digit", ite);
      }
      while (is_digit (ite)) ss << * ite ++;
      return ss.str ();
  });

  // -?[1-9][0-9]|0
  inline auto parse_decimal_int_literal = rollback ([] (source_iterator & ite)
  {
    skip_spaces (ite);
    auto first = ite;

    // -?
    constexpr auto parse_unary_minus = parse_char ('-');
    parse_unary_minus (ite);

    // [1-9]
    constexpr auto parse_1_to_9 = parse_char (std::make_pair ('1', '9'));
    if (not parse_1_to_9 (ite))
    {
      throw exception {"expected digit, but not digit", ite};
    }

    // [0-9]*
    while (is_digit (ite)) ++ ite;

    // suffix check
    if (is_letter_tail (ite))
    {
      throw exception {"unknown literal suffix.", ite};
    }

    return std::string {first, ite};
  });

  inline auto parse_int_literal (source_iterator & ite)
  {
    skip_spaces (ite);
    return parse_decimal_int_literal (ite);
  };

  inline auto parse_keyword (const std::string & word, source_iterator & ite) -> bool
  {
    auto ite_backup = ite;
    for (auto && elem : word)
    {
      if (ite.is_eof ())
      {
        throw exception ("parse_keyword: too short", ite);
      }
      if (elem != * ite)
      {
        ite = ite_backup;
        throw exception ("parse_keyword: expected keyword ", ite);
      }
      ++ ite;
    }
    return true;
  }
} // namespace kizuna::parser

#endif // KIZUNA_PARSER_PARSER_HPP
