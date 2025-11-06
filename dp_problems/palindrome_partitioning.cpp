# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;

bool isPalidrome(string s){
    string ps = s;
    reverse(ps.begin(), ps.end());
    return (ps == s);
}

// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, const string &s){    // front partitioning
    int n = s.size();
    // base cases
    if(i == n){
        return 0;
    }
    string temp = "";
    int minCost = 1e9;
    // core recursion logic
    for(int j = i ; j < n ; j++){
        temp += s[j];
        if(isPalidrome(temp)){
            int cost = 1 + recursion_approach(j + 1, s);
            minCost = min(minCost, cost);
        }
    }
    return minCost;
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// (BOTTOM-UP approach) (TC : O(n^2 * m), SC : O(n))
int hyper_optimized_approach(const string &s){
    int n = s.size();
    vector<int> dp(n+1, 0);

    dp[n] = 0;  // base case

    for(int i = n-1 ; i >= 0 ; i--){
        string temp = "";
        int minCost = 1e9;
        for(int j = i ; j < n ; j++){
            temp += s[j];
            if(isPalidrome(temp)){
                int cost = 1 + dp[j+1];
                minCost = min(minCost, cost);
            }
        }
        dp[i] = minCost;
    }
    return dp[0];
}

void printArray(const vector<char> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}
int main(){
    string s;
    cout << "Enter string: ";
    cin >> s;

    cout << "String : " << s << endl;

    cout << "Min partition cost : " << hyper_optimized_approach(s) - 1 << endl;
    return 0;
}