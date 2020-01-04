#ifndef KIZUNA_PARSER_EXCEPTION_HPP
#define KIZUNA_PARSER_EXCEPTION_HPP

#include <vector>
#include <tuple>
#include <sstream>

#include <kizuna/parser/source.hpp>
#include <kizuna/escape_sequence.hpp>
#include <kizuna/operator.hpp>

namespace kizuna::parser
{
  class parser_error
    : orable <parser_error>
  {
    struct parser_error_impl
    {
    private:
      std::string message;
      source_iterator ite;

      // 現在位置の(行の文字列),(何行目),(何文字目)を求める
      // O(N)
      auto get_current_line_col (const source_t & source) const
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
        while (current_line_end != source.end () && * current_line_end != '\n' && * current_line_end != '\r') ++ current_line_end;
        return std::make_tuple (std::string {current_line_begin, current_line_end}, line, col);
      }

    public:
      explicit parser_error_impl (const std::string & message_, const source_iterator & ite_)
        : message {message_}
        , ite {ite_}
      {}

      auto generate_error (std::ostream & out, const source_t & source) const -> decltype (auto)
      {
        using namespace escape_sequence;
        auto [current_line, line, col] = get_current_line_col (source);
        out << bold << source.filename << ":" << line << ":" << col << ": " << (bold | red) << "error" << reset << bold << ": " << message << std::endl;
        out << reset << current_line << std::endl;
        for (decltype (col) i = 1; i < col; ++ i)
        {
          out << ' ';
        }
        out << (bold | yellow) << '^' << reset;
        return out;
      }
    };

    std::vector <parser_error_impl> errors;

  public:
    explicit parser_error (const std::string & message, const source_iterator & ite)
      : errors {parser_error_impl {message, ite}}
    {}

    auto operator |= (const parser_error & e) -> decltype (auto)
    {
      for (auto && elem : e.errors)
      {
        errors.push_back (elem);
      }
      return * this;
    }

    auto what (const source_t & source) const
    {
      std::ostringstream ss;
      for (auto && elem : errors)
      {
        elem.generate_error (ss, source) << std::endl;
      }
      return ss.str ();
    }
  };
} // namespace kizuna::parser

#endif // KIZUNA_PARSER_EXCEPTION_HPP
