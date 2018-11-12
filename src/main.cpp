#include <iostream>
#include <sstream>
#include <string>
#include <utility> // std::move
#include <cstdint> // std::int64_t
#include <stdexcept> // std::runtime_error

namespace kizuna
{

  class input_stream
  {
  private:

    std::string source;

    std::string::iterator ite;
    std::string::iterator end;

    std::string filename;
    std::int64_t line;
    std::int64_t col;


    // 現在の行を取得。エラーメッセージ用
    // => std::string
    auto current_line () const
    {
      auto tmpite = ite - col + 1;
      auto tmpend = ite;
      while (tmpend != end && * tmpend != '\r' && * tmpend != '\n')
      {
        ++ tmpend;
      }
      return std::string {tmpite , tmpend};
    }


  public:

    input_stream (const std::string & s , const std::string & n = "(temporary string)")
      : source {s}
      , ite {source.begin ()}
      , end {source.end ()}
      , filename {n}
      , line {1}
      , col {1}
    {}


    input_stream (std::string && s , std::string && n = "(temporary string)") noexcept
      : source {std::move (s)}
      , ite {source.begin ()}
      , end {source.end ()}
      , filename {std::move (n)}
      , line {1}
      , col {1}
    {}


    input_stream (std::istream & fin , const std::string & n = "(temporary string)")
      : source {(std::istreambuf_iterator <char> (fin)) , std::istreambuf_iterator <char> ()}
      , ite {source.begin ()}
      , end {source.end ()}
      , filename {n}
      , line {1}
      , col {1}
    {}


    // なんかいい感じのエラーメッセージを生成する
    // 雑すぎない？
    auto generate_error (const std::string & message , int error_level = 2) const
    {
      static const char * error_string [] = {"\033[1;32msuggestion: " , "\033[1;33mwarning: " , "\033[1;31merror: "};
      std::ostringstream ss;
      ss << "\033[1m" << filename << ":" << line << ":" << col << ": " << error_string [error_level] << "\033[0m\033[1m" << message << std::endl;
      ss << "\033[0m" << current_line () << std::endl;
      for (decltype (col) i = 1; i < col; ++ i)
      {
        ss << ' ';
      }
      ss << "\033[1;33m^\033[0m";
      return std::runtime_error {ss.str ()};
    }


    auto is_eof () const noexcept
    {
      return ite == end;
    }


    // 残りの文字列
    // デバッグ用ですよたぶん
    // => std::string
    auto rest () const
    {
      return std::string {ite , end};
    }


    // 世間ではこれをゲッターという。
    auto get_iterator () const
    {
      return ite;
    }


    // 1文字スキップ
    // EOFだった場合は何もしない
    // 改行コードを通過したときにlineとcolをアレする
    auto skip1 () noexcept
    {
      if (ite != end)
      {
        if (* ite == '\r')
        {
          ++ ite;
          ++ line;
          col = 1;
          if (ite != end && * ite == '\n')
          {
            ++ ite;
          }
        }
        else if (* ite == '\n')
        {
          ++ ite;
          ++ line;
          col = 1;
        }
        else
        {
          ++ ite;
          ++ col;
        }
      }
    }


    auto peek () const
    {
      if (is_eof ())
      {
        throw generate_error ("unexpected EOF");
      }
      return (* ite);
    }


    auto get ()
    {
      auto && res = peek ();
      skip1 ();
      return res;
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
      return ! is_eof () && (is_alpha () || peek () == '_');
    }


    auto is_letter_tail () const noexcept
    {
      return ! is_eof () && (is_letter () || is_digit () || peek () == '-');
    }


    auto is_endline () const noexcept
    {
      return ! is_eof () && (peek () == '\n' || peek () == '\r');
    }


    auto is_space () const noexcept
    {
      return ! is_eof () && (peek () == ' ' || peek () == '\t' || is_endline ());
    }


    // 空白をスキップ
    auto skip_spaces () noexcept
    {
      while (is_space ())
      {
        skip1 ();
      }
    }


    // 改行コード以外の空白をスキップ
    // なんだかんだでこの言語の特徴になりそうな気がしないでもない
    auto skip_spaces_noendline () noexcept
    {
      while (is_space () && ! is_endline ())
      {
        skip1 ();
      }
    }


    // 改行コード以外をスキップ
    // 行コメントとかで使う・・・かも
    auto skip_noendline () noexcept
    {
      while (! is_eof () && ! is_endline ())
      {
        skip1 ();
      }
    }


  };


  inline auto parse_eof (input_stream & s)
  {
    s.skip_spaces ();
    if (! s.is_eof ())
    {
      throw s.generate_error ("not EOF");
    }
  }


  inline auto parse_anychar (input_stream & s)
  {
    return s.get ();
  }


  inline auto parse_decimal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    if ('1' <= s.peek () && s.peek () <= '9')
    {
      auto begin = s.get_iterator ();
      do
      {
        s.skip1 ();
      } while (s.is_digit ());
      return std::string {begin , s.get_iterator ()};
    }
    else
    {
      throw s.generate_error ("not decimal int literal");
    }
  }


  inline auto parse_octal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    if (s.peek () == '0')
    {
      auto begin = s.get_iterator ();
      do
      {
        s.skip1 ();
      } while ('0' <= s.peek () && s.peek () <= '7');
      return std::string {begin , s.get_iterator ()};
    }
    else
    {
      throw s.generate_error ("not octal int literal");
    }
  }


  inline auto parse_hexadecimal_int_literal (input_stream & s)
  {
    s.skip_spaces ();
    if (s.peek () == '0')
    {
      auto begin = s.get_iterator ();
      s.skip1 ();
      if (s.peek () == 'x' || s.peek () == 'X')
      {
        s.skip1 ();
      }
      else
      {
        throw s.generate_error ("not hexadecimal int literal");
      }
      if (s.is_digit () || ('A' <= s.peek () && s.peek () <= 'F') || ('a' <= s.peek () && s.peek () <= 'f'))
      {
        s.skip1 ();
      }
      else
      {
        throw s.generate_error ("not hexadecimal int literal");
      }
      while (s.is_digit () || ('A' <= s.peek () && s.peek () <= 'F') || ('a' <= s.peek () && s.peek () <= 'f'))
      {
        s.skip1 ();
      }
      return std::string {begin , s.get_iterator ()};
    }
    else
    {
      throw s.generate_error ("not hexadecimal int literal");
    }
  }


  inline auto parse_int_literal (input_stream & s)
  {
    auto backup = s;
    std::ostringstream ss;
    try
    {
      auto res = parse_decimal_int_literal (s);
      return res;
    }
    catch (const std::runtime_error & e)
    {
      s = backup;
      ss << e.what () << std::endl;
    }
    try
    {
      auto res = parse_hexadecimal_int_literal (s);
      return res;
    }
    catch (const std::runtime_error & e)
    {
      s = backup;
      ss << e.what () << std::endl;
    }
    try
    {
      auto res = parse_octal_int_literal (s);
      return res;
    }
    catch (const std::runtime_error & e)
    {
      ss << e.what () << std::flush;
      throw std::runtime_error {ss.str ()};
    }
  }


} // namespace kizuna


inline auto parse (kizuna::input_stream & s)
{
  auto res = kizuna::parse_int_literal (s);
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
  catch (std::runtime_error & e)
  {
    std::cerr << e.what () << std::endl;
  }



}
