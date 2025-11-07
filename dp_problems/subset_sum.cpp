# include <iostream>
# include <algorithm>
# include <vector>
# include <climits>

using namespace std;

// NAIVE 
// @explanation: Brute-force recursion
// @complexity: Time: O(2^n), Space: O(n) for recursion stack.
int recursion_approach(int idx, int target, vector<int> &arr){
    if(target == 0){
        return true;
    }
    if(idx == 0){
        return (arr[0] == target);
    }
    bool notTake = recursion_approach(idx - 1, target, arr);
    bool take = false;
    if(target >= arr[idx]){
        take = recursion_approach(idx - 1, target - arr[idx], arr);
    };
    return take || notTake;
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n * target), Space: O(target).
int hyper_optimized_approach(vector<int> &arr, int target){
    int n = arr.size();
    vector<bool> prev(target + 1, false);
    
    // base cases
    prev[0] = true;
    if(arr[0] <= target){
        prev[arr[0]] = true;
    }
    for(int idx = 1 ; idx < n ; idx++){
        for(int t = target ; t >= 0 ; t--){
            prev[t] = prev[t] || (t >= arr[idx] ? prev[t - arr[idx]] : false);
        }
    }
    return prev[target];
}

bool tabulation_approach(int n, int target, vector<int> &arr){
    vector<vector<bool>> dp(n, vector<bool>(target + 1, 0));
    // writing the base cases
    for(int i = 0 ; i < n ; i++){
        dp[i][0] = true;
    }
    dp[0][arr[0]] = true;
    for(int idx = 1 ; idx < n ; idx++){
        for(int t = 1 ; t <= target ; t++){
            bool notTake = dp[idx - 1][t];
            bool take = false;
            if(target >= arr[idx]){
                take = dp[idx - 1][t - arr[idx]];
            }
            dp[idx][t] = take | notTake;
        }   
    }
    cout << "Time Complexity: O(n * target)" << endl;
    cout << "Space Complexity: O(n * target)  (no auxilliary space)" << endl;
    return dp[n - 1][target];
}

// PROMPT: You are given an array of integers and a target value. Implement a space-optimized dynamic programming approach to determine whether there exists at least one subset of the given array whose elements sum exactly to the target value. Return true if such a subset exists, otherwise false.
int main() {
    vector<int> arr = {3, 34, 4, 12, 5, 2};
    int target = 9;

    cout << (hyper_optimized_approach(arr, target) ? "Yes" : "No") << endl;

    return 0;
}