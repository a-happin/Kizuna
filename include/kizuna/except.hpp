#ifndef KIZUNA_EXCEPT
#define KIZUNA_EXCEPT

#include <kizuna/utility.hpp>
#include <stdexcept>

#define KIZUNA_MACRO_DECLARE_EXCEPTION(x,super) \
  struct x : super \
  { \
    constexpr x () noexcept \
      : super ("kizuna::" #x) \
    {} \
    using super::super; \
  }\



namespace kizuna
{

  struct exception
  {

  private:

    using message_type = const char *;

    message_type message;

  public:

    constexpr exception () noexcept
      : message {"kizuna::exception"}
    {}

    constexpr exception (const char * msg) noexcept
      : message {msg}
    {}

    // copy constructor
    // constexpr exception (const exception &) noexcept = default;

    // move constructor
    // constexpr exception (exception &&) noexcept = default;

    constexpr auto what () const noexcept -> const char *
    {
      return message;
    }
  };

  KIZUNA_MACRO_DECLARE_EXCEPTION (runtime_error , exception);
  KIZUNA_MACRO_DECLARE_EXCEPTION (parse_error , exception);
  KIZUNA_MACRO_DECLARE_EXCEPTION (parse_eof_error , parse_error);

} // namespace kizuna

#undef KIZUNA_MACRO_DECLARE_EXCEPTION
#endif

