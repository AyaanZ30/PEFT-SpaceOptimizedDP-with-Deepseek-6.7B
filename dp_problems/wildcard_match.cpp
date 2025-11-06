# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE
bool recursion_approach(int i, int j, string &s1, string &s2){
    // base cases 
    if(i < 0 && j < 0){           // if both s1 and s2 have been exhausted (match success)
        return true;
    }
    if(i < 0 && j >= 0){
        return false;            // ""abcd and "aced" -> comparision is anyways over
    }
    if(j < 0 && i >= 0){                      // interesting case
        // (s1)"***" and (s2)"" ==> match only if remaining s1 is all * (check ground rules) , else no match
        for(int k = i ; k >= 0 ; k--){
            if(s1[k] != '*'){
                return false;
            } 
        }return true;
    }
    if(s1[i] == s2[j] || s1[i] == '?'){
        // ab? and abd are matching (so move both indexes backwards)
        return recursion_approach(i-1, j-1, s1, s2);
    }
    if(s1[i] == '*'){    
    // ab* and abcd => [* is considered empty](ab and abcd) or [* is not considered empty](ab* and abc)
        return recursion_approach(i-1, j, s1, s2) || recursion_approach(i, j-1, s1, s2);
    }
    return false;       // aec and abc => when comparing 'e' and 'b' scenario (strings are ofc not matching)
    cout << "TC : O(Exponential)" << endl;
    cout << "SC : O(N + M) (auxiliary)" << endl;
}

// (BOTTOM-UP)
bool tabulation_approach(string &s1, string &s2){
    int n = s1.size();
    int m = s2.size();
    vector<vector<bool>> dp(n+1, vector<bool>(m+1, false));     // 1-based indexing

    // cases analogous to recursion base cases
    dp[0][0] = true;
    for(int j = 1 ; j <= m ; j++){
        dp[0][j] = false;
    }
    for(int i = 1 ; i <= n ; i++){
        bool flag = true;
        for(int k = 0 ; k <= i ; k++){
            if(s1[k] != '*'){
                flag = !flag;
                dp[i][0] = flag;
            }
        }
        dp[i][0] = flag;
    }
    // main logic
    for(int i = 1 ; i <= n ; i++){
        for(int j = 1 ; j <= m ; j++){
            if(s1[i-1] == s2[j-1] || s1[i-1] == '?'){
                dp[i][j] = dp[i-1][j-1];
            }else if(s1[i-1] == '*'){
                dp[i][j] = dp[i-1][j] || dp[i][j-1];
            }else{
                dp[i][j] = false;
            }
        }
    }return dp[n][m];
}

// OPTIMIZED 
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n * m) , Space:  O(m)
bool hyper_optimized_approach(string &s1, string &s2){    // convert s1 -> s2 
    int m = s2.size();
    int n = s1.size();
    vector<bool> prev(m+1, false), curr(m+1, false);

    prev[0] = true;
    for(int j = 1 ; j <= m ; j++){
        prev[j] = false;
    }
    for(int i = 1 ; i <= n ; i++){
        // for every row 'i' you are assiging value at [0]  (first column : [i][0])
        bool flag = true;
        for(int k = 0 ; k <= i ; k++){
            if(s1[k] != '*'){
                flag = false;
                break;
            }
        }
        curr[0] = flag;       

        for(int j = 1 ; j <= m ; j++){
            if(s1[i-1] == s2[j-1] || s1[i-1] == '?'){
                curr[j] = prev[j-1];
            }else if(s1[i-1] == '*'){
                curr[j] = prev[j] || curr[j-1];
            }else{
                curr[j] = false;
            }
        }
        prev = curr;
    }
    return prev[m];
}

int main(){
    string s1 = "a*b", s2 = "ab";
    cout << "Ground rules : " << endl;
    cout << "? -> Matches with a single character (a-z)" << endl;
    cout << "* -> Matches with a seq of length >= 0" << endl;

    cout << "s1 : " << s1 << endl;
    cout << "s2 : " << s2 << endl;

    cout << "\nMatching ? : " << hyper_optimized_approach(s1, s2) << endl;
    return 0;
}
