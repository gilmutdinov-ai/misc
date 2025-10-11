#pragma once

#include <memory>
#include <string>
#include <vector>

namespace misc {

class Trie {
public:
  Trie() = default;
  Trie(const std::vector<std::string> &ws) {
    for (const auto &w : ws)
      insert(w);
  }

  void insert(std::string word) { insert_(0, word); }
  bool search(std::string word) { return search_(0, word, true); }
  bool startsWith(std::string prefix) { return search_(0, prefix, false); }
  bool isWord() const { return m_word_end; }

  std::shared_ptr<Trie> &child(char c) { return m_child[c - 'a']; }

private:
  void insert_(size_t i, const std::string &word) {

    if (i == word.size()) {
      m_word_end = true;
      return;
    }

    if (!child(word[i])) {
      child(word[i]).reset(new Trie{});
    }
    child(word[i])->insert_(i + 1, word);
  }

  bool search_(size_t i, const std::string &word, bool exact) {
    if (i == word.size()) {
      if (exact && !m_word_end)
        return false;
      return true;
    }

    if (word[i] != '.') {
      if (!child(word[i]))
        return false;

      return child(word[i])->search_(i + 1, word, exact);
    }
    // '.' - match any
    for (auto &ch : m_child)
      if (ch) {
        if (ch->search_(i + 1, word, exact))
          return true;
      }
    return false;
  }

private:
  bool m_word_end{false};
  std::shared_ptr<Trie> m_child[26];
};

} // namespace misc
