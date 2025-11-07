# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE
int recursion_approach(int idx1, int idx2, string &s1, string &s2){
    // base case(returns either 1 or 0 to determine the count)
    if(idx2 < 0){
        return 1;   // Found a valid subsequence
    }
    if(idx1 < 0 && idx2 >= 0){
        return 0;      // s1 exhausted, but s2 is not
    }
    // exploring all possibilities and summing up the results
    if(s1[idx1] == s2[idx2]){
        return recursion_approach(idx1 - 1, idx2 - 1, s1, s2) + recursion_approach(idx1 - 1, idx2, s1, s2);
    }
    return recursion_approach(idx1 - 1, idx2, s1, s2);
    cout << "Time Complexity: O(Exponential)" << endl;
    cout << "Space Complexity: O(n + m)  (recursion stack space)" << endl;
}

// memoization (due to overlapping subproblems)  (TOP-DOWN)
int memoization_approach(int i, int j, string &s1, string &s2, vector<vector<int>> dp){
    // base cases same as recursion approach
    if(j < 0){
        return 1;
    }
    if(i < 0 && j >= 0){
        return 0;
    }
    if(dp[i][j] != -1){            // avoid already solved problem
        return dp[i][j];
    }
    if(s1[i] == s2[j]){
        return dp[i][j] = memoization_approach(i-1, j-1, s1, s2, dp) + memoization_approach(i-1, j, s1, s2, dp);
    }
    return dp[i][j] = memoization_approach(i-1, j, s1, s2, dp);
    cout << "TC : O(N x M)" << endl; 
    cout << "SC : O(N x M) + O(N x M)" << endl; 
} 

// (BOTTOM-UP)
int tabulation_approach(string &s1, string &s2){
    vector<vector<int>> dp(s1.size() + 1, vector<int>(s2.size() + 1, 0)); // 1-based indexing (table size inc by 1 to deal with negative indexes)
    // base cases -> dp logic (to avoid recursion calls)
    for(int i = 0 ; i <= s1.size() ; i++){
        dp[i][0] = 1;                        // analogous to finding a valid subseq 
    }
    for(int j = 1 ; j <= s2.size() ; j++){
        dp[0][j] = 0;                         // analogous to case where s1 was exhausted (out-of-index) bu not s2
    }
    // going from start(bottom) -> end(up) 
    for(int i = 1 ; i <= s1.size() ; i++){
        for(int j = 1 ; j <= s2.size() ; j++){
            if(s1[i-1] == s2[j-1]){
                dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
            }else{
                dp[i][j] = dp[i-1][j];
            }
        }
    }return dp[s1.size()][s2.size()];      // last box contains optimal solution
    cout << "TC : O(N + M + N*M)" << endl; 
    cout << "SC : O(N x M)" << endl; 
}

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n * m) , Space:  O(m)
int hyper_optimized_approach(string &s1, string &s2){    // convert s1 -> s2 
    int n = s1.size();
    int m = s2.size();
    vector<int> prev(m + 1, 0);

    // dp logic -> 1-D array logic
    prev[0] = 1;

    for(int i = 0 ; i <= n ; i++){
        for(int j = m ; j >= 1 ; j--){
            if(s1[i-1] == s2[j-1]){
                prev[j] = prev[j-1] + prev[j];
            }else{
                continue;    // no change (same value remains)
            }
        }
    }return prev[m];
}

// PROMPT: Given two strings s and t, implement a space-optimized dynamic programming approach to count how many distinct subsequences of s are equal to t.
int main(){
    string s1 = "abcababbcabcbab";
    string s2 = "abc";

    cout << "\nOccurences : " << hyper_optimized_approach(s1, s2) << endl;
    return 0;
}