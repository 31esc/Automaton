#include <algorithm>
#include <bitset>
#include <cstddef>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>

using Edge = std::pair<char, std::vector<size_t>>;
using Edges = std::vector<std::unordered_map<char, std::vector<size_t>>>;
using EdgesForClasses =
    std::pair<std::pair<size_t, std::map<char, size_t>>, size_t>;

class Automaton {
 private:
  struct EdgeHelper {
    size_t from;
    size_t to;
    char symbol;

    EdgeHelper() = default;

    EdgeHelper(size_t from, size_t to, char symbol)
        : from(from), to(to), symbol(symbol) {}
  };

 private:
  static constexpr char kEps = '1';
  static constexpr size_t kMaxVertex = 1000;

  size_t start_;
  std::string alphabet_;
  size_t vertexes_count_;
  std::set<size_t> terminal_vertexes_;
  Edges edges_;

  static bool IsEps(const char symbol);

  static char GetSymbolOfEdge(Edge edge);

  static const std::vector<size_t> GetNeighborsOfEdge(Edge edge);

  std::vector<EdgeHelper> GetEdgesList(
      std::set<size_t> edges_to_remove = std::set<size_t>());

  std::vector<size_t> GetCompressedList(const std::vector<EdgeHelper>& pairs);

  size_t GetNewVertexNumber(const std::vector<size_t>& compressed_list,
                            const size_t vertex);

  void NewEdgesAfterCompression(const std::vector<EdgeHelper>& pairs,
                                const std::vector<size_t>& compressed_list);

  void NewTermsAfterCompression(const std::vector<Automaton::EdgeHelper>& pairs,
                                const std::vector<size_t>& compressed_list);

  void CompressAndAssignEdges(const std::vector<EdgeHelper>& pairs);

  void RemoveReachLessVertex();

  bool GetBit(size_t mask, size_t pos);

  std::string UnionStrings(std::string first, std::string second);

 public:
  Automaton() = default;

  explicit Automaton(const char symbol);

  explicit Automaton(const std::string& regex);

  size_t GetVertexCount() const;

  std::string GetAlphabet() const;

  void AddEdge(size_t from, size_t to, char symbol);

  void RemoveEpsEdges();

  void ToDFA();  // Deterministic Finite Automaton

  void ToCDFA();  // Complete Deterministic Finite Automaton

  void ToMCDFA();  // Minimal Complete Deterministic Finite Automaton

  void AdditionToCDFA();  // Addition to Complete Deterministic Finite Automaton

  friend Automaton operator+(const Automaton& first, const Automaton& second);

  friend Automaton operator-(const Automaton& first, const Automaton& second);

  friend Automaton operator*(const Automaton& first);

  friend std::istream& operator>>(std::istream& in, Automaton&);

  friend std::ostream& operator<<(std::ostream& out, const Automaton&);

  friend bool operator==(const Automaton& first, const Automaton& second);
};
