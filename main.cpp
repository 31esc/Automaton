#include <iostream>
#include "lib/automaton.hpp"

int main() {
  std::string s = "ab+*aa.ab+*.b.b.ab.a.+.b*.a.b*.";
  Automaton a(s);
  a.ToMCDFA();
  std::cout << a;
}