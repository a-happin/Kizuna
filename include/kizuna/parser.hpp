#ifndef KIZUNA_PARSER_HPP
#define KIZUNA_PARSER_HPP

#include <sstream>
#include <string>

#include <kizuna/input_stream.hpp>
#include <kizuna/exception.hpp>

namespace kizuna::parser
{

  struct eof_error : exception
  {
    eof_error (const input_stream & x)
      : exception {x.generate_error ("expected EOF, but not EOF")}
    {}
    using exception::what;
  };


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


  struct not_octal_character_error : exception
  {
    not_octal_character_error (const input_stream & x)
      : exception {x.generate_error ("8進数ではない文字")}
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


  struct not_hexadecimal_character_error : exception
  {
    not_hexadecimal_character_error (const input_stream & x)
      : exception {x.generate_error ("16進数ではない文字")}
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


  struct not_illegal_double_literal_error : exception
  {
    not_illegal_double_literal_error (const input_stream & x)
      : exception {x.generate_error ("不正なdouble型リテラル")}
    {}
    using exception::what;
  };


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


  struct not_word_error : exception
  {
    not_word_error (const input_stream & x)
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
      throw not_octal_character_error {s};
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
      throw not_hexadecimal_character_error {s};
    }

    // [0-9A-Fa-f]*
    while (s.is_digit () || ('A' <= s.peek () && s.peek () <= 'F') || ('a' <= s.peek () && s.peek () <= 'f'))
    {
      s.skip1 ();
    }

    // suffix check
    if (s.is_letter ())
    {
      throw not_hexadecimal_character_error {s};
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

    throw not_int_literal_error (s);
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
      // s = backup;
    }

    throw not_literal_error {s};
  }


  // [A-Za-z_][0-9A-Za-z_-]*
  inline auto parse_word (input_stream & s)
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
      throw not_word_error {s};
    }

    // [0-9A-Za-z_-]*
    while (s.is_letter_tail ())
    {
      s.skip1 ();
    }

    return std::string {begin , s.get_iterator ()};
  }

};

#endif


