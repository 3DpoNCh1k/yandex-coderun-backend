#include "algo/utils/io.hpp"
#include "algo/utils/join.hpp"
#include <algorithm>
#include <iostream>

#include <algo/utils/types/fundamentals.hpp>
#include <cassert>
#include <cstdlib>
#include <set>
#include <vector>

using namespace std;

struct Graph {
  int n_person;
  int n_topping;

  vector<vector<int>> topping_to_persons_plus;
  vector<vector<int>> topping_to_persons_minus;

  vector<set<int>> person_to_vertex;

  vector<bool> visited;
  bool inversed;

  vector<int> state;

  Graph(int n, int m, bool inversed)
      : n_person(n), n_topping(m), inversed(inversed) {
    topping_to_persons_plus.resize(n_topping);
    topping_to_persons_minus.resize(n_topping);

    person_to_vertex.resize(n_person);

    visited.resize(Size());

    state.resize(Size(), 0);
  }

  void AddPlus(int p, int t) {
    topping_to_persons_plus[t].push_back(p);
    if (inversed) {
      person_to_vertex[p].insert(Negate(t));
    } else {
      person_to_vertex[p].insert(t);
    }
  }

  void AddMinus(int p, int t) {
    topping_to_persons_minus[t].push_back(p);
    if (inversed) {
      person_to_vertex[p].insert(t);
    } else {
      person_to_vertex[p].insert(Negate(t));
    }
  }

  int Negate(int t) { return t < n_topping ? t + n_topping : t - n_topping; }

  int Size() { return n_topping * 2; }

  bool IsVisited(int v) { return visited[v]; }

  void MarkVisited(int v) {
    visited[v] = true;
    int t = v < n_topping ? v : v - n_topping;
    bool take_plus = v < n_topping ? true : false;
    take_plus ^= inversed;

    const vector<int> &persons =
        take_plus ? topping_to_persons_plus[t] : topping_to_persons_minus[t];

    for (int p : persons) {
      assert(person_to_vertex[p].find(v) != person_to_vertex[p].end());
      person_to_vertex[p].erase(v);
    }
  }

  int GetNextNeighbor(int v) {
    int t = v < n_topping ? v : v - n_topping;
    bool take_plus = v < n_topping ? false : true;
    take_plus ^= inversed;

    const vector<int> &persons =
        take_plus ? topping_to_persons_plus[t] : topping_to_persons_minus[t];

    int index = state[v];
    for (int i = index; i < persons.size(); ++i) {
      int p = persons[i];
      for (int u : person_to_vertex[p]) {
        if (u == Negate(v)) {
          continue;
        }
        assert(u != v);

        state[v] = i;
        return u;
      }
    }

    state[v] = persons.size();
    return -1;
  }
};

struct SCC {
  using Component = vector<int>;

  explicit SCC(Graph &g0, Graph &g_inversed0)
      : g_(g0), g_inversed_(g_inversed0), n_(g_.Size()) {}

  // Kosaraju's algorithm
  auto Condense() {
    OrderVertices();
    ExtractComponents();
    return Harvest();
  }

  void OrderVertices() {
    order_.clear();
    for (int v = 0; v < n_; ++v) {
      if (!g_.IsVisited(v)) {
        OrderDfs(v);
      }
    }
  }

  void ExtractComponents() {
    component_.assign(n_, -1);
    k_component_ = 0;
    for (int i = n_ - 1; i >= 0; --i) {
      int v = order_[i];
      if (!g_inversed_.IsVisited(v)) {
        ComponentDfs(v, k_component_);
        k_component_++;
      }
    }
  }

  vector<Component> Harvest() {
    vector<Component> components;
    components.resize(k_component_);
    for (int v = 0; v < n_; ++v) {
      components[component_[v]].push_back(v);
    }

    return std::move(components);
  };

  void OrderDfs(int v) {
    g_.MarkVisited(v);
    while (true) {
      int u = g_.GetNextNeighbor(v);
      if (u == -1) {
        break;
      }
      assert(!g_.IsVisited(u));
      OrderDfs(u);
    }
    order_.push_back(v);
  }

  void ComponentDfs(int v, int component_number) {
    component_[v] = component_number;
    g_inversed_.MarkVisited(v);
    while (true) {
      int u = g_inversed_.GetNextNeighbor(v);
      if (u == -1) {
        break;
      }
      assert(!g_inversed_.IsVisited(u));
      ComponentDfs(u, component_number);
    }
  }

  Graph &g_;
  Graph &g_inversed_;
  const int n_;

  std::vector<int> order_;
  std::vector<int> component_;
  int k_component_;
};

struct TwoSat {
  using Solution = std::vector<int>;
  using Result = std::pair<bool, Solution>;
  explicit TwoSat(int n) : k_variable_(n) {}

  Result Solve(const vector<vector<int>> &components) {
    value_.assign(k_variable_ * 2, -1);
    for (const auto &component : components) {
      if (value_[component[0]] == -1) {
        for (int v : component) {
          if (value_[v] != -1) {
            return Result(false, {});
          }
          value_[v] = 0;
          value_[VertexNegation(v)] = 1;
        }
      }
    }
    Solution solution;
    for (int v = 0; v < k_variable_; ++v) {
      if (value_[v] == 1) {
        solution.push_back(v + 1);
      }
    }
    return Result(true, solution);
  }

  int VertexNegation(int vertex) {
    if (vertex < k_variable_) {
      return vertex + k_variable_;
    }
    return vertex - k_variable_;
  }

  int k_variable_;
  std::vector<int> value_;
};

int main() {
  algo::utils::io::UnsyncIO();

  int n, m, k;
  cin >> n >> m >> k;

  auto g = Graph(n, m, false);
  auto g_inversed = Graph(n, m, true);

  for (int i = 0; i < k; ++i) {
    int a, b;
    cin >> a >> b;
    int p = a - 1;
    if (b > 0) {
      int t = b - 1;
      g.AddPlus(p, t);
      g_inversed.AddPlus(p, t);
    } else {
      int t = -b - 1;
      g.AddMinus(p, t);
      g_inversed.AddMinus(p, t);
    }
  }

  auto scc = SCC(g, g_inversed);
  auto components = scc.Condense();
  auto two_sat = TwoSat(m);
  auto [ok, solution] = two_sat.Solve(components);

  if (ok) {
    cout << solution.size() << endl;
    cout << algo::utils::JoinToString(solution, " ") << endl;
  } else {
    cout << -1 << endl;
  }
}
