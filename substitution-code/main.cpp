#include "algo/utils/io.hpp"
#include "algo/utils/join.hpp"
#include <bitset>
#include <cassert>
#include <iostream>

#include <algo/strings/aho_corasick/aho_corasick.hpp>
#include <algo/strings/aho_corasick/statistics/all_occurrences.hpp>
#include <algo/utils/types/fundamentals.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace algo::strings::aho_corasick;

static const int MAX_CHAR = 126;

using String = std::u32string;
using AllOccurrences = statistics::AllOccurrences<String>;

struct Data {
  vector<int> chars;
  vector<int> idx_to_positions_index;
  vector<vector<int>> char_to_positions;
  vector<String> char_to_diffs;
};

Data Process(const string &s) {
  auto data = Data{};
  int n = s.size();
  data.idx_to_positions_index.resize(n);
  data.char_to_positions.resize(MAX_CHAR + 1);
  data.char_to_diffs.resize(MAX_CHAR + 1);

  for (int i = 0; i < n; ++i) {
    auto ch = s[i];
    auto &positions = data.char_to_positions[ch];
    data.idx_to_positions_index[i] = positions.size();
    positions.push_back(i);
  }

  for (int ch = 0; ch <= MAX_CHAR; ++ch) {
    auto &positions = data.char_to_positions[ch];
    auto &diffs = data.char_to_diffs[ch];
    if (!positions.empty()) {
      data.chars.push_back(ch);
    }
    for (int i = 1; i < positions.size(); ++i) {
      auto diff = positions[i] - positions[i - 1];
      assert(diff >= 0);
      diffs.push_back(diff);
    }
  }

  return data;
};

int main() {
  algo::utils::io::UnsyncIO();

  string t, s;
  cin >> t;
  cin >> s;
  assert(t.size() >= s.size());

  auto t_data = Process(t);
  auto s_data = Process(s);

  vector<String> patterns;

  for (int ch : s_data.chars) {
    auto &pattern = s_data.char_to_diffs[ch];
    if (pattern.empty()) {
      continue;
    }

    patterns.push_back(pattern);
  }

  auto aho_corasick = AhoCorasick<Statistics<AllOccurrences>, String>(patterns);

  vector<vector<bitset<MAX_CHAR + 1>>> match_table(MAX_CHAR + 1);

  for (int ch = 0; ch <= MAX_CHAR; ++ch) {
    auto &text = t_data.char_to_diffs[ch];
    match_table[ch].resize(text.size());
  }

  auto match = [&match_table](int text_index, int position, int pattern_index) {
    match_table[text_index][position][pattern_index] = true;
  };

  auto is_match = [&match_table](int text_index, int position,
                                 int pattern_index) {
    return match_table[text_index][position][pattern_index];
  };

  for (int text_index : t_data.chars) {
    auto &text = t_data.char_to_diffs[text_index];
    if (text.empty()) {
      continue;
    }

    aho_corasick.ResetAllStatistics();
    aho_corasick.Scan(text);

    int i = 0;
    for (int pattern_index : s_data.chars) {
      auto &pattern = s_data.char_to_diffs[pattern_index];
      if (pattern.empty()) {
        continue;
      }
      auto stat = aho_corasick.Get<AllOccurrences>(i);
      for (auto position : stat.positions) {
        match(text_index, position, pattern_index);
      }
      i++;
    }
  }

  vector<int> answer;
  vector<int> mapping(MAX_CHAR + 1, 0);
  for (int i = 0; i <= t.size() - s.size(); ++i) {
    bool ok = true;
    // map
    for (int ch_s : s_data.chars) {
      int position_in_s = s_data.char_to_positions[ch_s][0];
      int position_in_t = i + position_in_s;
      int ch_t = t[position_in_t];
      mapping[ch_t]++;
      if (mapping[ch_t] > 1) {
        ok = false;
        break;
      }
    }

    if (ok) {
      for (int ch_s : s_data.chars) {
        int position_in_s = s_data.char_to_positions[ch_s][0];
        int position_in_t = i + position_in_s;
        int ch_t = t[position_in_t];

        auto &text = t_data.char_to_diffs[ch_t];
        int text_i = t_data.idx_to_positions_index[position_in_t];

        auto &pattern = s_data.char_to_diffs[ch_s];
        int pattern_i = s_data.idx_to_positions_index[position_in_s];

        assert(pattern_i == 0);

        if (pattern_i == pattern.size()) {
          // empty
          continue;
        }

        if (text_i == text.size()) {
          // empty
          ok = false;
          break;
        }

        int text_index = ch_t;
        int pattern_index = ch_s;

        ok &= is_match(text_index, text_i, pattern_index);

        if (!ok) {
          break;
        }
      }
    }

    // clean
    for (int ch_s : s_data.chars) {
      int position_in_s = s_data.char_to_positions[ch_s][0];
      int position_in_t = i + position_in_s;
      int ch_t = t[position_in_t];
      mapping[ch_t] = 0;
    }

    if (ok) {
      answer.push_back(i + 1);
    }
  }

  cout << answer.size() << endl;
  cout << algo::utils::JoinToString(answer, " ") << endl;
}
