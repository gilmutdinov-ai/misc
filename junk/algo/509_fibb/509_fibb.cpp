#include <array>

using M = array<array<2>>;

void mul(M &a, const M &b) {
  M c;
  c[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
  c[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];

  // fix
  c[1][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
  c[1][1] = a[0][0] * b[0][0] + a[0][1] * b[1][0];

  a = c;
}

void pow(M &a, int p) {
  M b{a};
  for (int i = 2; i <= p; ++i)
    mul(a, b);
}

void nfibb(int n) {
  // init M
  M m{{1, 1}, {1, 0}};
  // m^(n-1)
  pow(m, n - 1);
  return m[0][0];
}

int main() { return 0; }
