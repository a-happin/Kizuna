#ifndef KIZUNA_OPTIONAL_HPP
#define KIZUNA_OPTIONAL_HPP

#include <optional>
#include <iterator>

namespace kizuna
{
  using std::optional;
  using std::nullopt;
} // namespace kizuna

namespace std
{
  template <typename T>
  constexpr auto begin (const std::optional <T> & opt) noexcept -> const T *
  {
    return opt ? & * opt : nullptr;
  }

  template <typename T>
  constexpr auto end (const std::optional <T> & opt) noexcept -> const T *
  {
    return opt ? & * opt + 1 : nullptr;
  }
} // namespace std

#endif // KIZUNA_OPTIONAL_HPP
