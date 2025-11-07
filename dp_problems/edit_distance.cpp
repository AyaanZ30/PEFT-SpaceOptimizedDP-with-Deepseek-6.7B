# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE
int recursion_approach(int i, int j, string &s1, string &s2){
    // base cases 
    if(i < 0){           // s1 has exhausted 
    // s1-> ""horse, s2 -> "ro" then to make "" -> "ro" we need 2(=idx2+1) insertions
        return (j + 1);
    }
    if(j < 0){           // s2 has exhausted 
    // s1 -> "horse", s2 -> "" then to make "horse" -> "" we need 5(=idx1 + 1) deletions
        return (i + 1);
    }
    if(s1[i] == s2[j]){
        // if current letters match, simply shrink the strings by -1 (no operations applied)
        return 0 + recursion_approach(i-1, j-1, s1, s2);
    }    
    int insert = 1 + recursion_approach(i, j-1, s1, s2);    // INSERTION (hypothetically inserted the letter s2[j] in s1 ahead of ith position)
    int del = 1 + recursion_approach(i-1, j, s1, s2);    // DELETION (delete s1[i] and move idx one step behind if it doesnt match with s2[j])
    int replace = 1 + recursion_approach(i-1, j-1, s1, s2);  // REPLACE (replace s1[i] with s2[j] if they dont match -> to match them and hence shrink idx by 1) 
    return min(min(insert, replace), del);
    cout << "TC : O(Exponential)" << endl;
    cout << "SC : 2 * O(N + M) (recursion + auxiliary)" << endl;
}

// (BOTTOM-UP)
int tabulation_approach(string &s1, string &s2){
    int n = s1.size();
    int m = s2.size();
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));

    // initial fillings (analogous to the recursion cases)
    for(int j = 0 ; j <= m ; j++){
        dp[0][j] = j;
    }
    for(int i = 0 ; i <= n ; i++){
        dp[i][0] = i;
    }
    for(int i = 1 ; i <= n ; i++){
        for(int j = 1 ; j <= m ; j++){
            if(s1[i-1] == s2[j-1]){
                dp[i][j] = dp[i-1][j-1];
            }else{
                dp[i][j] = 1 + min(dp[i][j-1], min(dp[i-1][j], dp[i-1][j-1]));
            }
        }
    }return dp[n][m];
    cout << "TC : O(N + M + N*M)" << endl;
    cout << "SC : O(N*M) (2D-DP grid)" << endl;
}

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n * m) , Space:  O(m)
int hyper_optimized_approach(string &s1, string &s2){    // convert s1 -> s2 
    int n = s1.size();
    int m = s2.size();
    vector<int> prev(m+1, 0), curr(m+1, 0);

    for(int j = 0 ; j <= m ; j++){
        prev[j] = j;
    }
    for(int i = 1 ; i <= n ; i++){
        curr[0] = i;
        for(int j = 1 ; j <= m ; j++){
            if(s1[i-1] == s2[j-1]){
                curr[j] = prev[j-1];
            }else{
                curr[j] = 1 + min(curr[j-1], min(prev[j], prev[j-1]));
            }
        }
        prev = curr;
    }
    return prev[m];
}

int main(){
    string s1 = "gross";
    string s2 = "grease";

    cout << "Allowed operations (Insert, Delete, Replace)" << endl;

    cout << "\nMin operations needed : " << hyper_optimized_approach(s1, s2) << endl;
    return 0;
}