#include "algo/ranges/segment_tree/details/trees/dynamic.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

#include <algo/debug/debug.hpp>
#include <algo/ranges/segment_tree/trees.hpp>
#include <algo/ranges/statistics/minimum.hpp>
#include <algo/ranges/statistics/value_of.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <vector>

using namespace std;
using namespace algo::utils;

struct AddArithmeticProgression {
  using Element = i64;
  using Range = algo::ranges::Range<i64>;
  using Index = Range::Index;

  AddArithmeticProgression(Range range, Element first, Element difference)
      : range(range), first(first), difference(difference) {}

  AddArithmeticProgression(Index l, Index r, Element first, Element difference)
      : range(l, r), first(first), difference(difference) {}

  AddArithmeticProgression Compose(AddArithmeticProgression update) const {
    assert(range == update.range);

    auto result = AddArithmeticProgression(*this);
    result.first += update.first;
    result.difference += update.difference;
    return result;
  };

  AddArithmeticProgression OnSubrange(Range subrange) const {
    assert(subrange.IsInside(range));

    return AddArithmeticProgression(
        subrange, first + (subrange.l - range.l) * difference, difference);
  }

  Range range;
  Element first;
  Element difference;
};

struct LazyUpdates {
  struct Node {
    using Range = algo::ranges::Range<i64>;
    using Index = Range::Index;
    using Update = AddArithmeticProgression;

    using Value = i64;

    Range range;
    Value value;
    std::optional<Update> node_update;

    Node(Index l, Index r, Value value) : range(l, r), value(value) {}

    void Apply(const Update &update) {
      if (node_update.has_value()) {
        node_update = node_update.value().Compose(update);
      } else {
        node_update = update;
      }
    }

    void Push(Node &left, Node &right) {
      if (node_update.has_value()) {
        left.Apply(node_update.value().OnSubrange(left.range));
        right.Apply(node_update.value().OnSubrange(right.range));
        node_update.reset();
      }
    };
  };

  using Tree = algo::ranges::segment_tree::details::DynamicTree<Node>;

  struct LazyPropagator {
    using TreeNode = typename Tree::TreeNode;
    using Node = typename Tree::DataNode;
    using Update = typename Node::Update;
    using Index = typename Node::Index;
    using Range = typename Node::Range;

    Tree &tree;
    explicit LazyPropagator(Tree &tree) : tree(tree) {}

    void ApplyOnRange(const Update &update) {
      ApplyOnRangeImpl(tree.nodes[tree.ROOT], update);
    }

    void ApplyOnRangeImpl(Node &node, const Update &update) {
      if (node.range.IsOutside(update.range)) {
        return;
      }
      if (node.range.IsInside(update.range)) {
        node.Apply(update.OnSubrange(node.range));
        return;
      }
      node.Push(tree.GetLeft(node), tree.GetRight(node));
      ApplyOnRangeImpl(tree.GetLeft(node), update);
      ApplyOnRangeImpl(tree.GetRight(node), update);
    }

    auto Get() { return GetImpl(tree.nodes[tree.ROOT]); }

    i64 GetImpl(TreeNode &node) {
      if (node.left_index == -1) {
        auto result = node.value;
        if (node.node_update.has_value()) {
          auto update = node.node_update.value();
          if (update.difference > 0) {
            return update.first;
          }
          return update.first + update.difference * (node.range.Length() - 1);
        }
        return result;
      }
      node.Push(tree.GetLeft(node), tree.GetRight(node));
      auto left = GetImpl(tree.nodes[node.left_index]);
      auto right = GetImpl(tree.nodes[node.right_index]);
      return std::min(left, right);
    }
  };

  Tree tree;
  LazyPropagator propagator;

  LazyUpdates(i64 l, i64 r) : tree(l, r, 0), propagator(tree){};

  void Update(int l, int r, int first, int diff) {
    propagator.ApplyOnRange(AddArithmeticProgression(l, r, first, diff));
  }

  i64 GetMin() { return propagator.Get(); }
};

int main() {
  int n, m;
  cin >> n >> m;
  int L, R;
  cin >> L >> R;
  vector<pair<int, int>> blues(n);
  for (int i = 0; i < n; ++i) {
    int l, r;
    cin >> l >> r;
    blues[i] = {l, r};
  }
  vector<pair<int, int>> reds(m);
  for (int i = 0; i < m; ++i) {
    int l, r;
    cin >> l >> r;
    reds[i] = {l, r};
  }

  sort(reds.begin(), reds.end());
  int min_l = reds.begin()->first;
  int shift_from = L - min_l;

  int max_r = reds.rbegin()->second;
  int shift_to = R - max_r;

  assert(shift_from <= 0);
  assert(shift_to >= 0);

  auto ds = LazyUpdates(shift_from, shift_to);

  auto update = [shift_from, shift_to, &ds](int l, int r, int start,
                                            int increase) {
    auto bounded_l = max(l, shift_from);
    auto bounded_r = min(r, shift_to);
    if (bounded_l > bounded_r) {
      return;
    }
    auto bounded_start = start + (bounded_l - l) * increase;
    ds.Update(bounded_l, bounded_r, bounded_start, increase);
  };

  for (auto [red_l, red_r] : reds) {
    for (auto [blue_l, blue_r] : blues) {
      auto red_sz = red_r - red_l;
      auto blue_sz = blue_r - blue_l;
      int k = min(red_sz, blue_sz);

      int left_shift = blue_l - red_r;
      int right_shift = blue_r - red_l;

      // not zero changes
      left_shift++;
      right_shift--;

      int shift_cnt = right_shift - left_shift + 1;
      int min_changes_cnt = 2 * (k - 1) + 1;
      assert(shift_cnt >= min_changes_cnt);

      // 1..k-1
      int left_k_index = left_shift + (k - 1);
      // k-1..1
      int right_k_index = right_shift - (k - 1);
      // k..k

      update(left_shift, left_k_index - 1, 1, 1);
      update(right_k_index + 1, right_shift, k - 1, -1);
      update(left_k_index, right_k_index, k, 0);
    }
  }

  cout << ds.GetMin() << endl;
}
