# Automaton

Implementation of a Deterministic Finite Automaton, a Complete Deterministic Finite Automaton, a Minimal Complete Deterministic Finite Automaton, an addition to Complete Deterministic Finite Automaton.

The `IsSuffixByLetterFixLength` function determines whether, for a given regular expression $A$, letter $x$, and a natural number $k$, there are words in language L that contain the suffix $x^k$.

Arguments
$A$ is a string in the alphabet **{a, b, c, 1, ., +}** representing a regular expression in reverse Polish notation. Here, the operator **+** denotes union, **.** denotes concatenation, and **\*** denotes iteration (we do not support "exponentiation +"). For example, the string **ab+\*c.** represents the regular expression **(a+b)\*c**.
