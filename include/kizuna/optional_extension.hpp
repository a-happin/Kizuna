#ifndef KIZUNA_OPTIONAL_EXTENSION_HPP
#define KIZUNA_OPTIONAL_EXTENSION_HPP

#include <optional>

namespace kizuna::optional_extension
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
} // namespace kizuna::optional_extension

#endif // KIZUNA_OPTIONAL_EXTENSION_HPP
