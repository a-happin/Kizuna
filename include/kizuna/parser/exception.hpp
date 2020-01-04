#ifndef KIZUNA_PARSER_EXCEPTION_HPP
#define KIZUNA_PARSER_EXCEPTION_HPP

#include <kizuna/parser/source.hpp>

namespace kizuna::parser
{
  struct exception
  {
    std::string message;
    source_t::iterator ite;

    explicit exception (const std::string & message_, const source_t::iterator & ite_)
      : message {message_}
      , ite {ite_}
    {}
  };
} // namespace kizuna::parser

#endif // KIZUNA_PARSER_EXCEPTION_HPP
