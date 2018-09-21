#ifndef KIZUNA_AST
#define KIZUNA_AST
#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace kizuna::ast
{
  struct ast_base
  {
    std::string token;
    std::string type;
    std::vector <std::unique_ptr <ast_base>> children;
  };
} // namespace kizuna::ast

#endif

