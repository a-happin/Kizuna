#ifndef KIZUNA_ESCAPE_SEQUENCE_HPP
#define KIZUNA_ESCAPE_SEQUENCE_HPP

#include <iostream>
#include <iterator>
#include <kizuna/join.hpp>
#include <kizuna/stddef.hpp>
#include <kizuna/macro.hpp>

namespace kizuna::escape_sequence
{
  template <size_t N>
  struct escape_sequence_t
  {
    int values [N];

    template <size_t M>
    friend constexpr auto operator | (const escape_sequence_t & left, const escape_sequence_t <M> & right)
    {
      escape_sequence_t <N + M> res;
      for (size_t i = 0; i < N; ++ i) res.values [i] = left.values [i];
      for (size_t i = 0; i < M; ++ i) res.values [i + N] = right.values [i];
      return res;
    }

    friend auto operator << (std::ostream & out, const escape_sequence_t & self) -> decltype (auto)
    {
      using std::begin;
      using std::end;
      out << "\033[";
      ostream_join (KIZUNA_ALL (self.values), out, ";");
      out << "m";
      return out;
    }
  };

  struct reset_t : escape_sequence_t <1> {
    constexpr reset_t () noexcept
      : escape_sequence_t <1> ({0})
    {}
  };

  struct foreground_color_t : escape_sequence_t <1> {};

  struct background_color_t : escape_sequence_t <1> {};


  // --------------------------------
  // constant
  // --------------------------------
  constexpr reset_t reset;

  constexpr escape_sequence_t <1> bold      = {1};
  constexpr escape_sequence_t <1> italic    = {3};
  constexpr escape_sequence_t <1> underline = {4};
  constexpr escape_sequence_t <1> blink     = {5};
  constexpr escape_sequence_t <1> reverse   = {7};

  constexpr foreground_color_t black        = {{30}};
  constexpr foreground_color_t red          = {{31}};
  constexpr foreground_color_t green        = {{32}};
  constexpr foreground_color_t yellow       = {{33}};
  constexpr foreground_color_t blue         = {{34}};
  constexpr foreground_color_t magenta      = {{35}};
  constexpr foreground_color_t cyan         = {{36}};
  constexpr foreground_color_t white        = {{37}};
  constexpr foreground_color_t reset_color  = {{39}};
  constexpr background_color_t reset_background_color = {{49}};


  // --------------------------------
  // conversion
  // --------------------------------
  constexpr auto foreground (const foreground_color_t & x) noexcept -> foreground_color_t
  {
    return x;
  }
  constexpr auto foreground (const reset_t &) noexcept -> foreground_color_t
  {
    return reset_color;
  }
  constexpr auto background (const foreground_color_t & x) noexcept -> background_color_t
  {
    return background_color_t {{x.values [0] + 10}};
  }
  constexpr auto background (const reset_t &) noexcept -> background_color_t
  {
    return reset_background_color;
  }
} // namespace kizuna::escape_sequence

#endif // KIZUNA_ESCAPE_SEQUENCE_HPP
