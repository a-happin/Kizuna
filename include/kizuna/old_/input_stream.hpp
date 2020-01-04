#ifndef KIZUNA_INPUT_STREAM_HPP
#define KIZUNA_INPUT_STREAM_HPP
#include <sstream>
#include <string>
#include <cstdint> // std::int64_t

namespace kizuna
{


  inline constexpr int eof = -1;


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
    // () => std::string
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
      return ss.str ();
    }


    // 残りの文字列
    // デバッグ用ですよたぶん
    auto rest () const
    {
      return std::string {ite , end};
    }


    // 世間ではこれをゲッターという。
    auto get_iterator () const noexcept
    {
      return ite;
    }


    // experimental
    /*
    auto peek_throwable () const
    {
      if (ite == end)
      {
        throw eof_error {generate_error ("unexpected EOF")};
        throw generate_error ("unexpected EOF");
      }
      return * ite;
    }
    */


    // experimental
    // 前提：
    //   inline constexpr int eof = -1;
    auto peek () const noexcept -> int
    {
      return ite == end ? eof : * ite;
    }


    // experimental
    // 前提：
    //   inline constexpr struct eof_t {} eof;
    /*
    auto peek_variant () const noexcept -> std::variant <char , eof_t>
    {
      if (ite == end)
      {
        return eof_t {};
      }
      else
      {
        return * ite;
      }
    }
    */


    // 1文字スキップ
    // 改行コードを通過したときにlineとcolをアレする
    // EOFだった場合は何もしない
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


    auto get () noexcept
    {
      auto && res = peek ();
      skip1 ();
      return res;
    }


    // ファイル終端
    auto is_eof () const noexcept
    {
      return ite == end;
    }


    auto is_digit () const noexcept
    {
      // return ! is_eof () && '0' <= peek () && peek () <= '9';
      return '0' <= peek () && peek () <= '9';
    }


    auto is_upper () const noexcept
    {
      // return ! is_eof () && 'A' <= peek () && peek () <= 'Z';
      return 'A' <= peek () && peek () <= 'Z';
    }


    auto is_lower () const noexcept
    {
      // return ! is_eof () && 'a' <= peek () && peek () <= 'z';
      return 'a' <= peek () && peek () <= 'z';
    }


    auto is_alpha () const noexcept
    {
      // return ! is_eof () && (is_upper () || is_lower ());
      return is_upper () || is_lower ();
    }


    auto is_letter () const noexcept
    {
      // return ! is_eof () && (is_alpha () || peek () == '_');
      return is_alpha () || peek () == '_';
    }


    auto is_letter_tail () const noexcept
    {
      // return ! is_eof () && (is_letter () || is_digit () || peek () == '-');
      return is_letter () || is_digit () || peek () == '-';
    }


    auto is_endline () const noexcept
    {
      // return ! is_eof () && (peek () == '\n' || peek () == '\r');
      return peek () == '\n' || peek () == '\r';
    }


    auto is_space () const noexcept
    {
      // return ! is_eof () && (peek () == ' ' || peek () == '\t' || is_endline ());
      return peek () == ' ' || peek () == '\t' || is_endline ();
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

} // namespace kizuna

#endif


