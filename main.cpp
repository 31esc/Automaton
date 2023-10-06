#include <iostream>
#include "lib/automaton.hpp"

int main() {
  std::string s = "1ab+*b.+b.ab+*.";  // (1 + (a + b)*b)b(a+b)*
  Automaton a(s);
  a.ToMPDKA();
  std::cout << a;
}