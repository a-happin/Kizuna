#ifndef KIZUNA_PACKED_ITERATOR
#define KIZUNA_PACKED_ITERATOR
#include <string>


namespace kizuna
{

  struct packed_iterator
  {

    using value_type = std::string::iterator;

    value_type ite;
    value_type end;


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
      return * ite ++;
    }


    auto next ()
    {
      if (is_eof ())
      {
        throw "Error: out_of_range";
      }
      ++ ite;
    }



    auto is_char (char c) const noexcept
    {
      return ! is_eof () && peek () == c;
    }


    auto is_digit () const noexcept
    {
      return ! is_eof () && '0' < peek () && peek () < '9';
    }


    auto is_upper () const noexcept
    {
      return ! is_eof () && 'A' < peek () && peek () < 'Z';
    }


    auto is_lower () const noexcept
    {
      return ! is_eof () && 'a' < peek () && peek () < 'z';
    }


    auto is_alpha () const noexcept
    {
      return ! is_eof () && (is_upper () || is_lower ());
    }


    auto is_alphanum () const noexcept
    {
      return ! is_eof () && (is_alpha () || is_digit ());
    }


    auto is_letter () const noexcept
    {
      return ! is_eof () && (is_alphanum () || is_char ('_') || is_char ('-'));
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


    auto show () const
    {
      return std::string {ite , end};
    }
  };

  inline auto make_packed_iterator (std::string & s) noexcept
  {
    return packed_iterator {s.begin () , s.end ()};
  }
} // namespace kizuna

#endif

