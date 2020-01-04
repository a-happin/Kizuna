#include <iostream>
#include <string>
#include <utility> // std::move
#include <cstdint> // std::int64_t

#include <kizuna/parser.hpp>

using namespace std;

auto main () -> int
{
  cin.tie (nullptr);
  ios_base::sync_with_stdio (false);

  auto [result, rest] = kizuna::parse (cin);
  cout << "parsed: " << result << endl;
  cout << "rest: " << rest;
}
