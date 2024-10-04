#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

/*

это решение работает до n=6 включительно,
надо до 16

попробовать

https://www.geeksforgeeks.org/generate-n-bit-gray-codes/

https://www.geeksforgeeks.org/count-set-bits-in-an-integer/

*/

using namespace std;

class Solution {
public:
  using P = vector<int>;
  using PM = unordered_map<int, P>;
  using M = unordered_map<int, bool>;
  using G = unordered_map<int, vector<int>>;

  /*
    2^n == 111..111
    Построить граф G, куда можно идти из текущего числа (отнятием 1 бита)
  */

  void build_g(G &g, unsigned int n) {

    for (int c = pow(2, n) - 1; c > 0; --c) {

      for (unsigned int i = 1; i <= c; i <<= 1) {
        unsigned int t = c & ~i;
        if (t != 0 && c != t)
          g[c].push_back(t);
        if (t != c)
          g[t].push_back(c);
      }
    }
    for (auto &[i, v] : g) {
      sort(v.begin(), v.end());
    }
  }

  void print_g(const G &g) {
    for (const auto &[i, v] : g) {
      cout << i << ": ";
      for (const auto j : v) {
        cout << j << ", ";
      }
      cout << endl;
    }
  }

  int get_set_bits(unsigned int a) {
    int count = 0;
    while (a) {
      count += a & 1;
      a >>= 1;
    }
    return count;
  }

  int bit_diff(unsigned int a, unsigned int b) {
    unsigned int c = a ^ b;
    return get_set_bits(c);
  }

  inline bool is_pow2(unsigned int n) { return (n & (n - 1)) == 0; }

  void bt(int p2b, M &m, const G &g, P &p, bool &f) {

    if (p.size() == m.size() + 1) {
      if (bit_diff(p[0], p.back()) == 1)
        f = true;
      return;
    }

    // take i ?
    const auto &valid_go_v = g.at(p.back());
    for (const auto &i : valid_go_v) {
      if (!f && !m[i]) {
        if (is_pow2(i)) {
          if (p2b == 1 && p.size() < m.size() - 1)
            continue;
          --p2b;
        }
        p.push_back(i);
        m[i] = true;
        bt(p2b, m, g, p, f);
        if (f)
          return;
        m[i] = false;
        p.pop_back();
        if (is_pow2(i))
          ++p2b;
      }
    }
  }

  pair<M, int> build_map(int n) {
    M m;
    int p2{0};
    int mx = pow(2, n) - 1;
    for (int i = 1; i <= mx; ++i) {
      m[i] = false;
      if (is_pow2(i))
        ++p2;
    }
    return make_pair(m, p2);
  }

  P grayCode(int n) {
    P p;
    p.push_back(0);
    auto [m, p2] = build_map(n);
    G g;
    build_g(g, n);
    // print_g(g);
    bool f = false;
    cout << "built G and M" << endl;
    bt(p2, m, g, p, f);
    return p;
  }

  PM pm;
  void precalc(int n) { pm[n] = grayCode(n); }

  void precalc_all() {
    cout << "precalc_all" << endl;
    const int start = 6;
    const int finish = 10;
    for (int i = start; i <= finish; ++i) {
      cout << "precalc " << i << "..";
      precalc(i);
      cout << "DONE" << endl;
    }
  }
};

int main() {
  Solution s;
  s.precalc_all();
}
