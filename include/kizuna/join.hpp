#ifndef KIZUNA_JOIN_HPP
#define KIZUNA_JOIN_HPP

#include <iostream>

namespace kizuna
{
  template <typename Iterator>
  auto ostream_join (Iterator first, Iterator last, std::ostream & out, std::string_view delimiter = ", ") -> decltype (auto)
  {
    if (first != last) out << * first ++;
    while (first != last) out << delimiter << * first ++;
    return out;
  }
} // namespace kizuna

#endif // KIZUNA_JOIN_HPP
