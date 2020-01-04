#ifndef KIZUNA_EXCEPTION_HPP
#define KIZUNA_EXCEPTION_HPP

#include <string>

namespace kizuna
{
  struct exception
  {
    std::string message;

    auto what () const noexcept -> decltype (auto)
    {
      return message;
    }
  };
}

#endif

