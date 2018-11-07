#ifndef KIZUNA_UTILITY
#define KIZUNA_UTILITY
#include <iostream>
#include <string>
#include <type_traits>

namespace kizuna
{
  using size_t = decltype (sizeof (0));

  template <typename T , size_t N>
  using rawarray = T [N];
  // using rawarray = T [([] () constexpr {
  //  static_assert (N != static_cast <size_t> (0));
  //  return N;
  // } ())];
  // using rawarray = T [std::enable_if_t <N != static_cast <size_t> (0) , std::integral_constant <size_t , N>>::value];

  inline auto file_to_string (std::istream & fin)
  {
    return std::string {(std::istreambuf_iterator <char> (fin)) , std::istreambuf_iterator <char> ()};
  }
}

#endif

