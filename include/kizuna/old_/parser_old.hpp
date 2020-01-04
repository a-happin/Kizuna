#ifndef KIZUNA_PARSER_HPP
#define KIZUNA_PARSER_HPP

#include <string>

#include <kizuna/input_stream.hpp>
#include <kizuna/exception.hpp>


#define KIZUNA_ERROR_DEF(name,message) \
struct name : exception\
{\
  name (const input_stream & x)\
    : exception {x.generate_error ( message )}\
  {}\
  using exception::what;\
}

namespace kizuna::parser
{

  KIZUNA_ERROR_DEF (eof_error , "expected EOF , but not EOF");
  KIZUNA_ERROR_DEF (failed_parse_string_error , "parse failed");


  struct not_decimal_int_literal_error : exception
  {
    not_decimal_int_literal_error (const input_stream & x)
      : exception {x.generate_error ("not decimal int literal")}
    {}
    using exception::what;
  };


  struct not_octal_int_literal_error : exception
  {
    not_octal_int_literal_error (const input_stream & x)
      : exception {x.generate_error ("not octal int literal")}
    {}
    using exception::what;
  };


  struct not_octal_int_literal_error2 : exception
  {
    not_octal_int_literal_error2 (const input_stream & x)
      : exception {x.generate_error ("not octal int literal")}
    {}
    using exception::what;
  };


  struct not_hexadecimal_int_literal_error : exception
  {
    not_hexadecimal_int_literal_error (const input_stream & x)
      : exception {x.generate_error ("not hexadecimal int literal")}
    {}
    using exception::what;
  };


  struct not_hexadecimal_int_literal_error2 : exception
  {
    not_hexadecimal_int_literal_error2 (const input_stream & x)
      : exception {x.generate_error ("not hexadecimal int literal")}
    {}
    using exception::what;
  };


  struct not_int_literal_error : exception
  {
    not_int_literal_error (const input_stream & x)
      : exception {x.generate_error ("not int literal")}
    {}
    using exception::what;
  };


  struct not_double_literal_error : exception
  {
    not_double_literal_error (const input_stream & x)
      : exception {x.generate_error ("not double literal")}
    {}
    using exception::what;
  };


  KIZUNA_ERROR_DEF (not_bool_literal_error , "not bool literal");


  struct unknown_literal_suffix_error : exception
  {
    unknown_literal_suffix_error (const input_stream & x)
      : exception {x.generate_error ("unknown literal suffix")}
    {}
    using exception::what;
  };


  struct not_literal_error : exception
  {
    not_literal_error (const input_stream & x)
      : exception {x.generate_error ("not literal")}
    {}
    using exception::what;
  };


  struct not_identifier_error : exception
  {
    not_identifier_error (const input_stream & x)
      : exception {x.generate_error ("not word")}
    {}
    using exception::what;
  };


  // EOF
  inline auto parse_eof (input_stream & s)
  {
    s.skip_spaces ();
    if (! s.is_eof ())
    {
      throw eof_error {s};
    }
  }


  inline auto parse_anychar (input_stream & s)
  {
    return s.get ();
  }


  inline auto parse_string (const std::string & str , input_stream & s)
  {
    // auto backup = s;
    for (auto && elem : str)
    {
      if (s.peek () != elem)
      {
        // s = backup;
        throw failed_parse_string_error {s};
      }
      s.skip1 ();
    }
    return str;
  }


  KIZUNA_ERROR_DEF (not_keyword_error , "not keyword");

  inline auto parse_keyword (const std::string & str , input_stream & s)
  {
    s.skip_spaces ();
    for (auto && elem : str)
    {
      if (s.peek () != elem)
      {
        throw not_keyword_error {s};
      }
      s.skip1 ();
    }
    if (s.is_letter_tail ())
    {
      throw not_keyword_error {s};
    }
    return str;
  }


  // -?[1-9][0-9]*
  inline auto parse_decimal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    // -?
    if (s.peek () == '-')
    {
      s.skip1 ();
    }

    // [1-9]
    if ('1' <= s.peek () && s.peek () <= '9')
    {
      s.skip1 ();
    }
    else
    {
      throw not_decimal_int_literal_error {s};
    }

    // [0-9]*
    while (s.is_digit ())
    {
      s.skip1 ();
    }

    // suffix check
    if (s.is_letter ())
    {
      throw unknown_literal_suffix_error {s};
    }

    return std::string {begin , s.get_iterator ()};
  }


  // -?0[0-7]*
  inline auto parse_octal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    // -?
    if (s.peek () == '-')
    {
      s.skip1 ();
    }

    // 0
    if (s.peek () == '0')
    {
      s.skip1 ();
    }
    else
    {
      throw not_octal_int_literal_error {s};
    }

    // [0-7]*
    while ('0' <= s.peek () && s.peek () <= '7')
    {
      s.skip1 ();
    }

    // suffix check
    if (s.is_digit () || s.is_letter ())
    {
      throw not_octal_int_literal_error2 {s};
    }

    return std::string {begin , s.get_iterator ()};
  }


  // -?0[Xx][0-9A-Fa-f]+
  inline auto parse_hexadecimal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    // -?
    if (s.peek () == '-')
    {
      s.skip1 ();
    }

    // 0
    if (s.peek () == '0')
    {
      s.skip1 ();
    }
    else
    {
      throw not_hexadecimal_int_literal_error {s};
    }

    // [Xx]
    if (s.peek () == 'x' || s.peek () == 'X')
    {
      s.skip1 ();
    }
    else
    {
      throw not_hexadecimal_int_literal_error {s};
    }


    // [0-9A-Fa-f]
    if (s.is_digit () || ('A' <= s.peek () && s.peek () <= 'F') || ('a' <= s.peek () && s.peek () <= 'f'))
    {
      s.skip1 ();
    }
    else
    {
      throw not_hexadecimal_int_literal_error2 {s};
    }

    // [0-9A-Fa-f]*
    while (s.is_digit () || ('A' <= s.peek () && s.peek () <= 'F') || ('a' <= s.peek () && s.peek () <= 'f'))
    {
      s.skip1 ();
    }

    // suffix check
    if (s.is_letter ())
    {
      throw not_hexadecimal_int_literal_error2 {s};
    }

    return std::string {begin , s.get_iterator ()};
  }


  inline auto parse_int_literal (input_stream & s)
  {
    auto backup = s;

    try
    {
      return parse_hexadecimal_int_literal (s);
    }
    catch (const not_hexadecimal_int_literal_error &)
    {
      s = backup;
    }

    try
    {
      return parse_octal_int_literal (s);
    }
    catch (const not_octal_int_literal_error &)
    {
      s = backup;
    }

    try
    {
      return parse_decimal_int_literal (s);
    }
    catch (const not_decimal_int_literal_error &)
    {
      // s = backup;
    }

    throw not_int_literal_error {s};
  }


  // -?[0-9]+\.[0-9]+
  inline auto parse_double_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    // -?
    if (s.peek () == '-')
    {
      s.skip1 ();
    }

    // [0-9]+
    if (! s.is_digit ())
    {
      throw not_double_literal_error {s};
    }
    do
    {
      s.skip1 ();
    } while (s.is_digit ());

    // \.
    if (s.peek () == '.')
    {
      s.skip1 ();
    }
    else
    {
      throw not_double_literal_error {s};
    }

    // [0-9]+
    if (! s.is_digit ())
    {
      throw not_double_literal_error {s};
    }
    do
    {
      s.skip1 ();
    } while (s.is_digit ());

    // suffix check
    if (s.is_letter ())
    {
      throw unknown_literal_suffix_error {s};
    }

    return std::string {begin , s.get_iterator ()};
  }


  inline auto parse_bool_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto backup = s;

    try
    {
      auto res = parse_keyword ("true" , s);
      return res;
    }
    catch (const not_keyword_error &)
    {
      s = backup;
    }

    try
    {
      auto res = parse_keyword ("false" , s);
      return res;
    }
    catch (const not_keyword_error &)
    {
      throw not_bool_literal_error {s};
    }
  }


  KIZUNA_ERROR_DEF (not_string_literal_error , "not string literal");


  inline auto parse_string_literal (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    if (s.peek () == '"')
    {
      s.skip1 ();
    }
    else
    {
      throw not_string_literal_error {s};
    }

    while (! s.is_eof () && ! s.is_endline () && s.peek () != '"')
    {
      if (s.peek () == '\\')
      {
        s.skip1 ();
        if (! s.is_eof () && ! s.is_endline ())
        {
          s.skip1 ();
        }
        else
        {
          throw not_string_literal_error {s};
        }
      }
      else
      {
        s.skip1 ();
      }
    }

    if (s.peek () == '"')
    {
      s.skip1 ();
    }
    else
    {
      throw not_string_literal_error {s};
    }

    return std::string {begin , s.get_iterator ()};
  }


  inline auto parse_literal (input_stream & s)
  {
    auto backup = s;

    try
    {
      return parse_double_literal (s);
    }
    catch (const not_double_literal_error &)
    {
      s = backup;
    }

    try
    {
      return parse_int_literal (s);
    }
    catch (const not_int_literal_error &)
    {
      s = backup;
    }

    try
    {
      return parse_bool_literal (s);
    }
    catch (const not_bool_literal_error &)
    {
      s = backup;
    }

    try
    {
      return parse_string_literal (s);
    }
    catch (const not_string_literal_error &)
    {
      // s = backup;
    }

    throw not_literal_error {s};
  }


  // [A-Za-z_][0-9A-Za-z_-]*
  inline auto parse_identifier (input_stream & s)
  {
    s.skip_spaces ();
    auto begin = s.get_iterator ();

    // [A-Za-z_]
    if (s.is_letter ())
    {
      s.skip1 ();
    }
    else
    {
      throw not_identifier_error {s};
    }

    // [0-9A-Za-z_-]*
    while (s.is_letter_tail ())
    {
      s.skip1 ();
    }

    return std::string {begin , s.get_iterator ()};
  }

}

#undef KIZUNA_ERROR_DEF

#endif


