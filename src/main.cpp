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

  try
  {
    auto [result, rest] = kizuna::parse (cin, "(stdin)");
    cout << "parsed: " << result << endl;
    cout << "rest: " << rest;
  }
  catch (const std::runtime_error & e)
  {
    cerr << e.what () << endl;
  }
}
