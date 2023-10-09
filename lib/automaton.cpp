#include "automaton.hpp"

bool Automaton::IsEps(const char symbol) {
  return symbol == kEps;
}

char Automaton::GetSymbolOfEdge(Edge edge) {
  return edge.first;
}

const std::vector<size_t> Automaton::GetNeighborsOfEdge(Edge edge) {
  return edge.second;
}

size_t Automaton::GetVertexCount() const {
  return vertexes_count_;
}

std::string Automaton::GetAlphabet() const {
  return alphabet_;
}

void Automaton::AddEdge(size_t from, size_t to, char symbol) {
  edges_[from][symbol].push_back(to);
}

Automaton::Automaton(const char symbol) {
  start_ = 0;
  alphabet_ = "";

  if (symbol == '1') {
    vertexes_count_ = 1;
    terminal_vertexes_ = {0};
    edges_.resize(vertexes_count_);
  } else {
    alphabet_ += symbol;
    vertexes_count_ = 2;
    terminal_vertexes_ = {1};
    edges_.resize(vertexes_count_);
    edges_[0][symbol].push_back(1);
  }
}

Automaton::Automaton(const std::string& regex) {
  std::stack<Automaton> st;
  for (char current_symbol : regex) {
    if (current_symbol == '+') {
      if (st.empty()) {
        throw std::runtime_error("Incorrect regex");
      }

      Automaton second = st.top();
      st.pop();

      if (st.empty()) {
        throw std::runtime_error("Incorrect regex");
      }

      Automaton first = st.top();
      st.pop();

      st.push(first + second);

    } else if (current_symbol == '.') {
      if (st.empty()) {
        throw std::runtime_error("Incorrect regex");
      }

      Automaton second = st.top();
      st.pop();

      if (st.empty()) {
        throw std::runtime_error("Incorrect regex");
      }

      Automaton first = st.top();
      st.pop();

      st.push(first - second);

    } else if (current_symbol == '*') {
      if (st.empty()) {
        throw std::runtime_error("Incorrect regex");
      }

      Automaton first = st.top();
      st.pop();

      st.push(*first);

    } else {
      st.push(Automaton(current_symbol));
    }
  }
  if (st.empty() || st.size() > 1) {
    throw std::runtime_error("Incorrect regex");
  }
  *this = st.top();
}

std::string UnionStrings(std::string first, std::string second) {
  std::set<char> set;

  for (char c : first) {
    set.insert(c);
  }

  for (char c : second) {
    set.insert(c);
  }

  std::string result;
  for (char c : set) {
    result += c;
  }
  return result;
}

Automaton operator+(const Automaton& first, const Automaton& second) {
  Automaton result;
  const size_t first_shift = 1;
  const size_t second_shift = first.GetVertexCount() + 1;

  result.vertexes_count_ = 2 + first.GetVertexCount() + second.GetVertexCount();
  result.edges_.resize(result.vertexes_count_);
  result.start_ = 0;
  result.AddEdge(result.start_, first.start_ + first_shift, Automaton::kEps);
  result.AddEdge(result.start_, second.start_ + second_shift, Automaton::kEps);

  result.terminal_vertexes_ = {result.vertexes_count_ - 1};
  size_t first_term = *first.terminal_vertexes_.begin();
  size_t second_term = *second.terminal_vertexes_.begin();
  size_t res_term = *result.terminal_vertexes_.begin();
  result.AddEdge(first_term + first_shift, res_term, Automaton::kEps);
  result.AddEdge(second_term + second_shift, res_term, Automaton::kEps);

  result.alphabet_ = UnionStrings(first.GetAlphabet(), second.GetAlphabet());

  for (size_t first_vertex = 0; first_vertex < first.GetVertexCount();
       ++first_vertex) {
    for (auto edge : first.edges_[first_vertex]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        result.AddEdge(first_vertex + first_shift, to + first_shift, symbol);
      }
    }
  }

  for (size_t second_vertex = 0; second_vertex < second.GetVertexCount();
       ++second_vertex) {
    for (auto edge : second.edges_[second_vertex]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        result.AddEdge(second_vertex + second_shift, to + second_shift, symbol);
      }
    }
  }

  return result;
}

Automaton operator-(const Automaton& first, const Automaton& second) {
  Automaton result;
  result.start_ = first.start_;
  const size_t first_shift = 0;
  const size_t second_shift = first.GetVertexCount();
  result.vertexes_count_ = first.GetVertexCount() + second.GetVertexCount();
  result.edges_.resize(result.vertexes_count_);

  size_t first_term = *first.terminal_vertexes_.begin();
  size_t second_term = *second.terminal_vertexes_.begin();
  result.terminal_vertexes_ = {second_term + second_shift};
  result.AddEdge(first_term + first_shift, second.start_ + second_shift,
                 Automaton::kEps);

  result.alphabet_ = UnionStrings(first.GetAlphabet(), second.GetAlphabet());

  for (size_t first_vertex = 0; first_vertex < first.GetVertexCount();
       ++first_vertex) {
    for (auto edge : first.edges_[first_vertex]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        result.AddEdge(first_vertex + first_shift, to + first_shift, symbol);
      }
    }
  }

  for (size_t second_vertex = 0; second_vertex < second.GetVertexCount();
       ++second_vertex) {
    for (auto edge : second.edges_[second_vertex]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        result.AddEdge(second_vertex + second_shift, to + second_shift, symbol);
      }
    }
  }

  return result;
}

Automaton operator*(const Automaton& first) {
  Automaton result;
  result.start_ = 0;
  result.terminal_vertexes_ = {0};
  result.vertexes_count_ = first.GetVertexCount() + 1;
  result.edges_.resize(result.vertexes_count_);

  size_t first_term = *first.terminal_vertexes_.begin();
  const int first_shift = 1;
  result.AddEdge(result.start_, first.start_ + first_shift, Automaton::kEps);
  result.AddEdge(first_term + first_shift, result.start_, Automaton::kEps);

  result.alphabet_ = first.GetAlphabet();

  for (size_t first_vertex = 0; first_vertex < first.GetVertexCount();
       ++first_vertex) {
    for (auto edge : first.edges_[first_vertex]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        result.AddEdge(first_vertex + first_shift, to + first_shift, symbol);
      }
    }
  }

  return result;
}

std::istream& operator>>(std::istream& in, Automaton& automaton) {
  in >> automaton.start_;
  automaton.alphabet_ = "";
  size_t shift = 1;

  if (automaton.start_ + shift > automaton.edges_.size()) {
    automaton.edges_.resize(automaton.start_ + shift);
    automaton.vertexes_count_ = automaton.start_ + shift;
  }

  std::string empty_line;
  std::getline(in, empty_line);
  std::getline(in, empty_line);

  std::string term;
  while (std::getline(in, term) && !term.empty()) {
    automaton.terminal_vertexes_.insert(std::stoul(term));
    if (std::stoul(term) + shift > automaton.edges_.size()) {
      automaton.edges_.resize(std::stoul(term) + shift);
      automaton.vertexes_count_ = std::stoul(term) + shift;
    }
  }

  std::string edge;
  while (std::getline(in, edge) && !edge.empty()) {
    size_t from = 0;
    size_t to = 0;
    char symbol = '1';
    std::istringstream stream(edge);
    stream >> from >> to >> symbol;
    if (std::max(from, to) + shift > automaton.edges_.size()) {
      automaton.edges_.resize(std::max(from, to) + shift);
      automaton.vertexes_count_ = std::max(from, to) + shift;
    }

    if (!Automaton::IsEps(symbol)) {
      automaton.alphabet_ += symbol;
    }
    automaton.alphabet_ = UnionStrings(automaton.GetAlphabet(), "");

    automaton.AddEdge(from, to, symbol);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Automaton& automaton) {
  out << automaton.start_ << "\n\n";
  for (size_t v : automaton.terminal_vertexes_) {
    out << v << '\n';
  }
  out << "\n";

  for (size_t from = 0; from < automaton.vertexes_count_; ++from) {
    for (auto edge : automaton.edges_[from]) {
      char symbol = Automaton::GetSymbolOfEdge(edge);
      for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
        out << from << ' ' << to << ' ' << symbol << '\n';
      }
    }
  }
  return out;
}

bool operator==(const Automaton& first, const Automaton& second) {
  if (first.start_ != second.start_ || first.alphabet_ != second.alphabet_ ||
      first.vertexes_count_ != second.vertexes_count_ ||
      first.terminal_vertexes_ != second.terminal_vertexes_ ||
      first.edges_ != second.edges_) {
    return false;
  }
  return true;
}

std::vector<Automaton::EdgeHelper> Automaton::GetEdgesList(
    std::set<size_t> edges_to_remove) {
  std::vector<EdgeHelper> res;
  for (size_t from = 0; from < vertexes_count_; ++from) {
    for (auto edge : edges_[from]) {
      for (auto to : Automaton::GetNeighborsOfEdge(edge)) {
        if (edges_to_remove.count(from) || edges_to_remove.count(to)) {
          continue;
        }
        auto tmp = EdgeHelper(from, to, GetSymbolOfEdge(edge));
        res.emplace_back(tmp);
      }
    }
  }
  return res;
}

std::vector<size_t> Automaton::GetCompressedList(
    const std::vector<Automaton::EdgeHelper>& pairs) {
  std::vector<size_t> list_of_vertices;
  for (const EdgeHelper& edge : pairs) {
    list_of_vertices.push_back(edge.from);
    list_of_vertices.push_back(edge.to);
  }

  std::sort(list_of_vertices.begin(), list_of_vertices.end());

  list_of_vertices.erase(
      std::unique(list_of_vertices.begin(), list_of_vertices.end()),
      list_of_vertices.end());

  return list_of_vertices;
}

size_t Automaton::GetNewVertexNumber(const std::vector<size_t>& compressed_list,
                                     const size_t vertex) {
  return lower_bound(compressed_list.begin(), compressed_list.end(), vertex) -
         compressed_list.begin();
}

void Automaton::NewEdgesAfterCompression(
    const std::vector<Automaton::EdgeHelper>& pairs,
    const std::vector<size_t>& compressed_list) {
  edges_.clear();
  edges_.resize(vertexes_count_);

  for (const EdgeHelper& edge : pairs) {
    size_t from = GetNewVertexNumber(compressed_list, edge.from);
    size_t to = GetNewVertexNumber(compressed_list, edge.to);
    edges_[from][edge.symbol].push_back(to);
  }
}

void Automaton::NewTermsAfterCompression(
    const std::vector<Automaton::EdgeHelper>& pairs,
    const std::vector<size_t>& compressed_list) {
  std::set<size_t> new_terms;

  std::set<size_t> set;
  for (const EdgeHelper& edge : pairs) {
    set.insert(edge.from);
    set.insert(edge.to);
  }

  for (auto v : terminal_vertexes_) {
    if (set.count(v) == 1) {
      size_t new_number_of_v = GetNewVertexNumber(compressed_list, v);
      new_terms.insert(new_number_of_v);
    }
  }
  terminal_vertexes_ = new_terms;
}

void Automaton::CompressAndAssignEdges(
    const std::vector<Automaton::EdgeHelper>& pairs) {
  std::vector<size_t> compressed_list = GetCompressedList(pairs);
  vertexes_count_ = compressed_list.size();
  NewTermsAfterCompression(pairs, compressed_list);
  NewEdgesAfterCompression(pairs, compressed_list);
  start_ = GetNewVertexNumber(compressed_list, start_);
}

void Automaton::RemoveReachLessVertex() {
  std::set<size_t> edges_to_remove;

  std::vector<std::bitset<kMaxVertex>> is_reach(vertexes_count_, 0);
  for (size_t v = 0; v < vertexes_count_; ++v) {
    is_reach[v][v] = true;
  }

  for (size_t i = 0; i < vertexes_count_; ++i) {
    for (size_t v = 0; v < vertexes_count_; ++v) {
      for (auto edge : edges_[v]) {
        for (size_t to : Automaton::GetNeighborsOfEdge(edge)) {
          is_reach[v] |= is_reach[to];
        }
      }
    }
  }

  for (size_t v = 0; v < vertexes_count_; ++v) {
    if (!is_reach[start_][v]) {
      edges_to_remove.insert(v);
    }

    bool ok = true;
    for (size_t u = 0; u < vertexes_count_; ++u) {
      if (terminal_vertexes_.count(u) && is_reach[v][u]) {
        ok = true;
      }
    }

    if (!ok) {
      edges_to_remove.insert(v);
    }
  }

  auto list = GetEdgesList(edges_to_remove);
  CompressAndAssignEdges(list);
}

void Automaton::RemoveEpsEdges() {
  std::vector<std::bitset<kMaxVertex>> is_reach(vertexes_count_, 0);
  std::vector<std::vector<EdgeHelper>> eps_edges(vertexes_count_);

  for (size_t from = 0; from < vertexes_count_; ++from) {
    for (auto edge : edges_[from]) {
      char symbol = GetSymbolOfEdge(edge);
      for (size_t to : GetNeighborsOfEdge(edge)) {
        if (IsEps(symbol)) {
          eps_edges[from].push_back(EdgeHelper(from, to, symbol));
        }
      }
    }
    is_reach[from][from] = true;
  }

  for (size_t i = 0; i < vertexes_count_; ++i) {
    for (size_t v = 0; v < vertexes_count_; ++v) {
      for (const EdgeHelper& edge : eps_edges[v]) {
        size_t to = edge.to;
        is_reach[v] |= is_reach[to];
      }
    }
  }

  std::vector<std::unordered_map<char, std::vector<size_t>>> new_edges(
      vertexes_count_);
  for (size_t v = 0; v < vertexes_count_; ++v) {
    for (size_t u = 0; u < vertexes_count_; ++u) {
      if (is_reach[v][u]) {
        for (auto edge : edges_[u]) {
          char symbol = GetSymbolOfEdge(edge);
          for (size_t to : GetNeighborsOfEdge(edge)) {
            if (!IsEps(symbol)) {
              new_edges[v][symbol].push_back(to);
            }
          }
        }
        if (terminal_vertexes_.count(u)) {
          terminal_vertexes_.insert(v);
        }
      }
    }
  }
  edges_ = new_edges;
  RemoveReachLessVertex();
}

bool Automaton::GetBit(size_t mask, size_t pos) {
  return mask & (1 << pos);
}

void Automaton::ToDFA() {
  RemoveEpsEdges();
  std::vector<EdgeHelper> list;
  std::queue<size_t> q;
  q.push((1 << start_));
  start_ = (1 << start_);
  std::set<size_t> used;
  std::set<size_t> new_terms;

  while (!q.empty()) {
    size_t mask = q.front();
    q.pop();

    if (used.count(mask)) {
      continue;
    }

    for (size_t v = 0; v < vertexes_count_; ++v) {
      if (terminal_vertexes_.count(v) && GetBit(mask, v)) {
        new_terms.insert(mask);
        break;
      }
    }

    used.insert(mask);
    std::unordered_map<char, size_t> delta;
    for (size_t v = 0; v < vertexes_count_; ++v) {
      if (GetBit(mask, v)) {
        for (auto edge : edges_[v]) {
          char symbol = GetSymbolOfEdge(edge);
          for (size_t to : GetNeighborsOfEdge(edge)) {
            delta[symbol] |= (1 << to);
          }
        }
      }
    }

    for (auto el : delta) {
      list.emplace_back(EdgeHelper(mask, el.second, el.first));
      q.push(el.second);
    }
  }
  terminal_vertexes_ = new_terms;
  CompressAndAssignEdges(list);
}

void Automaton::ToCDFA() {
  ToDFA();

  auto list = GetEdgesList();
  size_t stok = vertexes_count_;
  bool is_need_stok = false;

  for (size_t v = 0; v < vertexes_count_; ++v) {
    std::set<char> used;
    for (auto edge : edges_[v]) {
      used.insert(GetSymbolOfEdge(edge));
    }

    for (char c : alphabet_) {
      if (!used.count(c)) {
        is_need_stok = true;
        list.emplace_back(EdgeHelper(v, stok, c));
      }
    }
  }

  if (is_need_stok) {
    for (char c : alphabet_) {
      list.emplace_back(EdgeHelper(stok, stok, c));
    }
  }
  CompressAndAssignEdges(list);
}

void Automaton::AdditionToCDFA() {
  ToCDFA();
  std::set<size_t> set;
  for (size_t from = 0; from < vertexes_count_; ++from) {
    for (auto edge : edges_[from]) {
      char symbol = GetSymbolOfEdge(edge);
      for (size_t to : GetNeighborsOfEdge(edge)) {
        set.insert(from);
        set.insert(to);
      }
    }
  }

  for (size_t term : terminal_vertexes_) {
    set.erase(term);
  }

  terminal_vertexes_ = set;
}

void Automaton::ToMCDFA() {
  ToCDFA();

  std::vector<size_t> classes(vertexes_count_, 0);
  for (auto v : terminal_vertexes_) {
    classes[v] = 1;
  }

  size_t number_of_classes = 2;
  for (size_t i = 0; i < vertexes_count_; ++i) {
    std::vector<EdgesForClasses> tmp_classes(vertexes_count_);

    for (size_t v = 0; v < vertexes_count_; ++v) {
      tmp_classes[v].first.first = classes[v];
      tmp_classes[v].second = v;

      for (auto edge : edges_[v]) {
        char symbol = GetSymbolOfEdge(edge);
        for (size_t to : GetNeighborsOfEdge(edge)) {
          tmp_classes[v].first.second[symbol] = classes[to];
        }
      }
    }

    std::sort(tmp_classes.begin(), tmp_classes.end());

    classes[tmp_classes[0].second] = 0;
    for (size_t v = 1; v < vertexes_count_; ++v) {
      if (tmp_classes[v].first == tmp_classes[v - 1].first) {
        classes[tmp_classes[v].second] = classes[tmp_classes[v - 1].second];
      } else {
        classes[tmp_classes[v].second] = classes[tmp_classes[v - 1].second] + 1;
      }
    }

    if (number_of_classes ==
        classes[tmp_classes[vertexes_count_ - 1].second] + 1) {
      break;
    }
    number_of_classes = classes[tmp_classes[vertexes_count_ - 1].second] + 1;
  }

  start_ = classes[start_];

  std::set<size_t> new_term_vertexes;
  for (size_t v : terminal_vertexes_) {
    new_term_vertexes.insert(classes[v]);
  }
  terminal_vertexes_ = new_term_vertexes;

  Edges old_edges = std::move(edges_);
  size_t old_vertexes_count = vertexes_count_;

  vertexes_count_ = number_of_classes;
  edges_.resize(vertexes_count_);

  std::set<size_t> set_for_classes;
  for (size_t v = 0; v < old_vertexes_count; ++v) {
    if (auto it = set_for_classes.find(classes[v]);
        it != set_for_classes.end()) {
      continue;
    }

    set_for_classes.insert(classes[v]);
    for (auto edge : old_edges[v]) {
      char symbol = GetSymbolOfEdge(edge);
      for (size_t to : GetNeighborsOfEdge(edge)) {
        AddEdge(classes[v], classes[to], symbol);
      }
    }
  }
}