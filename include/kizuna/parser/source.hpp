#ifndef KIZUNA_PARSER_SOURCE_HPP
#define KIZUNA_PARSER_SOURCE_HPP

#include <iostream>
#include <kizuna/iterator.hpp>

namespace kizuna::parser
{
  struct source_t
  {
    std::string content;
    std::string filename;

    struct iterator : input_iterator <iterator, char>
    {
    private:
      using inner_iterator_t = std::string::const_iterator;

      inner_iterator_t ite;
      inner_iterator_t end;

    public:
      //iterator () = delete;
      iterator (inner_iterator_t first, inner_iterator_t last)
        : ite {first}
        , end {last}
      {}

      auto is_eof () const noexcept
      {
        return ite == end;
      }

      auto operator * () const noexcept -> decltype (auto)
      {
        return * ite;
      }

      auto operator ++ () noexcept -> decltype (auto)
      {
        ++ ite;
        return * this;
      }

      friend auto operator == (const iterator & left, const iterator & right) noexcept
      {
        return left.ite == right.ite;
      }
    }; // struct iterator

    explicit source_t (std::istream & in, const std::string & filename_ = "(temporary string)")
      : content {std::istreambuf_iterator <char> (in), std::istreambuf_iterator <char> ()}
      , filename {filename_}
    {}

    auto begin () const
    {
      return iterator {content.begin (), content.end ()};
    }

    auto end () const
    {
      return iterator {content.end (), content.end ()};
    }
  };

  using source_iterator = source_t::iterator;
} // namespace kizuna::parser

#endif // KIZUNA_PARSER_SOURCE_HPP
