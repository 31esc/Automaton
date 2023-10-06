#include <fstream>
#include <iostream>
#include "automaton.hpp"
#include "gtest/gtest.h"

TEST(Regex_to_NKA, Throw) {
  std::string str = "abc";
  EXPECT_THROW(Automaton automaton(str), std::runtime_error);

  str = "a*.";
  EXPECT_THROW(Automaton automaton(str), std::runtime_error);

  str = "*.+";
  EXPECT_THROW(Automaton automaton(str), std::runtime_error);

  str = "ca.b.a+";  // cab + a
  EXPECT_NO_THROW(Automaton automaton(str));

  str = "ba+*";  // (b + a)*
  EXPECT_NO_THROW(Automaton automaton(str));

  str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  EXPECT_NO_THROW(Automaton automaton(str));

  str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  EXPECT_NO_THROW(Automaton automaton(str));
}

TEST(Regex_to_NKA, Сorrectness_1) {
  std::ifstream in("../tests/txt/NKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_NKA, Сorrectness_2) {
  std::ifstream in("../tests/txt/NKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_NKA_without_eps, Сorrectness_1) {
  std::ifstream in("../tests/txt/NKA_without_eps/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);
  automaton.RemoveEpsEdges();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_NKA_without_eps, Сorrectness_2) {
  std::ifstream in("../tests/txt/NKA_without_eps/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);
  automaton.RemoveEpsEdges();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(NKA_to_NKA_without_eps, Сorrectness_1) {
  std::ifstream in("../tests/txt/NKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;
  correct_automaton.RemoveEpsEdges();

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);
  automaton.RemoveEpsEdges();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(NKA_to_NKA_without_eps, Сorrectness_2) {
  std::ifstream in("../tests/txt/NKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;
  correct_automaton.RemoveEpsEdges();

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);
  automaton.RemoveEpsEdges();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_DKA, Сorrectness_1) {
  std::ifstream in("../tests/txt/DKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);
  automaton.ToDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_DKA, Сorrectness_2) {
  std::ifstream in("../tests/txt/DKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);
  automaton.ToDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_PDKA, Сorrectness_1) {
  std::ifstream in("../tests/txt/PDKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);
  automaton.ToPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_PDKA, Сorrectness_2) {
  std::ifstream in("../tests/txt/PDKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);
  automaton.ToPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(PDKA_to_addition_to_PDKA, Сorrectness_1) {
  std::ifstream in1("../tests/txt/PDKA/input3.txt",
                    std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in1 >> correct_automaton;

  std::ifstream in("../tests/txt/PDKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton automaton;
  in >> automaton;
  automaton.AdditionToPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(PDKA_to_addition_to_PDKA, Сorrectness_2) {
  std::ifstream in1("../tests/txt/PDKA/input4.txt",
                    std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in1 >> correct_automaton;

  std::ifstream in("../tests/txt/PDKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton automaton;
  in >> automaton;
  automaton.AdditionToPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_MPDKA, Сorrectness_1) {
  std::ifstream in("../tests/txt/MPDKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab+*c.ac+*.";  // (a + b)*c(a + c)*
  Automaton automaton(str);
  automaton.ToMPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(Regex_to_MPDKA, Сorrectness_2) {
  std::ifstream in("../tests/txt/MPDKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in >> correct_automaton;

  std::string str = "ab.ba.+*c.ca+*.";  // (ab + ba)*c(c + a)*
  Automaton automaton(str);
  automaton.ToMPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(PDKA_to_MPDKA, Сorrectness_1) {
  std::ifstream in1("../tests/txt/MPDKA/input1.txt",
                    std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton correct_automaton;
  in1 >> correct_automaton;

  std::ifstream in("../tests/txt/PDKA/input1.txt",
                   std::ifstream::in);  // (a + b)*c(a + c)*
  Automaton automaton;
  in >> automaton;
  automaton.ToMPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}

TEST(PDKA_to_MPDKA, Сorrectness_2) {
  std::ifstream in1("../tests/txt/MPDKA/input2.txt",
                    std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton correct_automaton;
  in1 >> correct_automaton;

  std::ifstream in("../tests/txt/PDKA/input2.txt",
                   std::ifstream::in);  // (ab + ba)*c(c + a)*
  Automaton automaton;
  in >> automaton;
  automaton.ToMPDKA();

  EXPECT_TRUE(automaton == correct_automaton);
}
