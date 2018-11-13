#ifndef KIZUNA_EXCEPTION_HPP
#define KIZUNA_EXCEPTION_HPP

#include <string>

namespace kizuna
{

  struct exception
  {

  private:
    std::string s;

  public:

    exception (const std::string & x)
      : s {x}
    {}

    exception (std::string && x) noexcept
      : s {std::move (x)}
    {}

    auto what () const noexcept -> const std::string &
    {
      return (s);
    }
  };

}

#endif


