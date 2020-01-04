#ifndef KIZUNA_STDDEF_HPP
#define KIZUNA_STDDEF_HPP

#include <cstddef>

namespace kizuna
{
  using ptrdiff_t = std::ptrdiff_t;
  using size_t = std::size_t;
  using nullptr_t = std::nullptr_t;
  using max_align_t = std::max_align_t;
  using byte = std::byte;
} // namespace kizuna

#endif // KIZUNA_STDDEF_HPP
