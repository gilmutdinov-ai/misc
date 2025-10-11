class Solution {
public:
  int dp_bottom_up(const string &t1, const string &t2) {
    vector<vector<int>> dp;
    {
      vector<int> m(t2.size() + 1, 0);
      dp.resize(t1.size() + 1, m);
    }
    const auto n = t1.size();
    const auto m = t2.size();
    for (auto i = 1; i <= n; ++i)
      for (auto j = 1; j <= m; ++j) {
        if (t1[i - 1] == t2[j - 1])
          dp[i][j] = dp[i - 1][j - 1] + 1;
        else
          dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    return dp[n][m];
  }

  int longestCommonSubsequence(string text1, string text2) {

    return dp_bottom_up(text1, text2);
  }
};