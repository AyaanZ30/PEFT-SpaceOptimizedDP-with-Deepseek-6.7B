# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

void printSCS(string &s1, string &s2, vector<vector<int>> &dp){
    string scs = "";
    int i = s1.size();
    int j = s1.size();

    while(i > 0 && j > 0){
        if(s1[i-1] == s2[j-1]){
            scs += s1[i-1];
            i--;
            j--;
        }else if(dp[i-1][j] > dp[i][j-1]){   
        // move up (s1 character at (i, j) eliminated from the picture (i -> i-1) is added to scs) (row shifted upwards, reducing string s1)
            scs += s1[i - 1];
            i--;
        }else{
        // move right (s2 character at (i, j) eliminated from the picture (j -> j-1) is added to scs) (column shifted rightwards, reducing string s2)
            scs += s2[j - 1];
            j--;
        }
    }
    while(i > 0){ scs += s1[i - 1]; i--; }
    while(j > 0){ scs += s2[j - 1]; j--; }

    reverse(scs.begin(), scs.end());
    cout << "SCS : " << scs;
}

// NAIVE
int recursion_approach(int idx1, int idx2, string &s1, string &s2){
    if(idx1 < 0 || idx2 < 0){
        return 0;
    }
    if(s1[idx1] == s2[idx2]){
        return (1 + recursion_approach(idx1 - 1, idx2 - 1, s1, s2));
    }
    cout << "Time Complexity: O(Exponential)" << endl;
    cout << "Space Complexity: O(n1 + n2)  (recursion stack space)" << endl;
    return max(recursion_approach(idx1 - 1, idx2, s1, s2), recursion_approach(idx1, idx2 - 1, s1, s2));
}

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n1 * n2) , Space:  O(min(n1, n2))
int hyper_optimized_approach(string &s1, string &s2){    // convert s1 -> s2 
    int n = s1.size();
    int m = s2.size();
    vector<int> prev(m + 1, 0), curr(m + 1, 0);

    // base cases
    for(int j = 0 ; j <= m ; j++){
        prev[j] = 0;
    }
    
    for(int i = 1 ; i <= n ; i++){
        for(int j = 1 ; j <= m ; j++){
            if(s1[i-1] == s2[j-1]){
                curr[j] = 1 + prev[j-1];
            }else{
                curr[j] = 0 + max(prev[j], curr[j-1]);
            }
        }
        prev = curr;
    }
    // s1 : brute, s2 : groot => len(scs) = len(s1) + len(s2) - len(lcs(s1, s2)) = 5 + 5 - 2 = 8 (bgruoote) (order maintained)
    int length_scs = (n + m - prev[m]);    // length(lcs) stored at prev[m] 
    return length_scs;
}

// (TOP-DOWN)
int memoization_approach(int idx1, int idx2, string &s1, string &s2, vector<vector<int>> &dp){
    if(idx1 < 0 || idx2 < 0){
        return 0;
    }
    if(dp[idx1][idx2] != -1){             // if this state is already computed, avoid recomputation
        return dp[idx1][idx2];
    }
    if(s1[idx1] == s2[idx2]){
        return dp[idx1][idx2] = 1 + memoization_approach(idx1 - 1, idx2 - 1, s1, s2, dp);
    }
    cout << "Time Complexity: O(n1 * n2)" << endl;
    cout << "Space Complexity: O(n1 * n2)  (2-D DP + recursion stack space)" << endl;
    return dp[idx1][idx2] = 0 + max(memoization_approach(idx1 - 1, idx2, s1, s2, dp), memoization_approach(idx1, idx2 - 1, s1, s2, dp));
}

// (BOTTOM-UP approach)
int tabulation_approach(string &s1, string &s2, vector<vector<int>> &dp){
    // base cases (aftr shifting index by 1 to avoid negative indexing)
    for(int i = 0 ; i <= s1.size() ; i++){
        dp[i][0] = 0;
    }
    for(int j = 0 ; j <= s2.size() ; j++){
        dp[0][j] = 0;
    }
    for(int i = 1 ; i <= s1.size() ; i++){
        for(int j = 1 ; j <= s2.size() ; j++){
            if(s1[i-1] == s2[j-1]){
                dp[i][j] = 1 + dp[i-1][j-1];
            }else{
                dp[i][j] = 0 + max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    cout << "Time Complexity: O(n1 * n2)" << endl;
    cout << "Space Complexity: O(n1 * n2) (2-D DP used)" << endl;
    printSCS(s1, s2, dp);
    return dp[s1.size()][s2.size()];
}

int main() {
    string s1 = "brute";
    string s2 = "groot";

    cout << "\nLength of SCS : " << hyper_optimized_approach(s1, s2) << endl;
    return 0;
}
