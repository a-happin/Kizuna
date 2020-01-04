#ifndef KIZUNA_MACRO_HPP
#define KIZUNA_MACRO_HPP

#define KIZUNA_DECLARE_DEFAULT_COPY_CONSTRUCTOR(classname) \
  classname (const classname &) = default; \
  auto operator = (const classname &) -> classname & = default;

#define KIZUNA_DECLARE_DEFAULT_MOVE_CONSTRUCTOR(classname) \
  classname (classname &&) noexcept = default; \
  auto operator = (classname &&) noexcept -> classname & = default;

#define KIZUNA_FOR_IMPL(i,b,e) for (std::common_type_t <decltype (b), decltype (e)> i = (b), i ## __end = (e); i < i ## __end; ++ i)
#define KIZUNA_FOR(...) KIZUNA_FOR_IMPL(__VA_ARGS__)
#define KIZUNA_REP(i,n) KIZUNA_FOR(i, std::size_t{}, n)
#define KIZUNA_ALL(x) begin (x), end (x)
#define KIZUNA_TAIL(x) std::next (begin (x)), end (x)

#define KIZUNA_TO_STRING(...) #__VA_ARGS__

#endif // KIZUNA_MACRO_HPP
