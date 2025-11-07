# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE (TC : O(2^n) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int prev, const vector<int> &arr){
    // base case (when you have traversed the entire array) 
    if(i == arr.size()){
        return 0;
    }
    int take = 0;
    int notTake = 0 + recursion_approach(i+1, prev, arr);
    if(arr[i] > arr[prev] || prev == -1){     // since 1st element (prev = -1) can be taken unconditionally
        take = 1 + recursion_approach(i+1, i, arr);
    }
    return max(take, notTake);
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n^2) , Space:  O(n)
int hyper_optimized_approach(vector<int> &arr){
    int n = arr.size();
    vector<int> dp(n, 1);
    int maxi = 1;
    
    // main logic (hash array to help in printing LIS sequence)
    for(int cur = 0 ; cur < n ; cur++){
        for(int prev = 0 ; prev < cur ; prev++){
            if(arr[cur] > arr[prev] && 1 + dp[prev] > dp[cur]){   // valid increasing
                dp[cur] = 1 + dp[prev];
            }
        }
        maxi = max(maxi, dp[cur]);
    }
    return maxi;
}

// (TOP-DOWN approach)
// TC : O(n^2) , SC : O(n^2) for dp + O(n) auxiliary stack space
int memoization_approach(int i, int prev, const vector<int> &arr, vector<vector<int>> &dp){
    // base case (when you have traversed the entire array)
    if(i == arr.size()){
        dp[i][prev] = 0;
    }
    if(dp[i][prev + 1] != -1){             // if this state is already computed, avoid recomputation
        return dp[i][prev + 1];
    }
    int take = 0;
    int notTake = 0 + memoization_approach(i + 1, prev, arr, dp);
    if(prev == -1 || arr[i] > arr[prev]){
        take = 1 + memoization_approach(i + 1, i, arr, dp);
    }
    return dp[i][prev + 1] = max(take, notTake);
}

// (BOTTOM-UP approach)
int tabulation_approach(const vector<int> &arr){
    int n = arr.size();
    vector<vector<int>> dp(n + 1 , vector<int>(n + 1, 0));

    // main logic
    for(int i = n-1 ; i >= 0 ; i--){
        for(int prev = i-1 ; prev >= -1 ; prev--){
            int len = 0 + dp[i-1][prev + 1];
            if(arr[i] > arr[prev] || prev == -1){
                len = max(len, 1 + dp[i + 1][i + 1]);
            }
            dp[i][prev + 1] = len;
        }
    }return dp[0][0];
}

void printArray(const vector<int> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}

// PROMPT: Given an integer array arr, implement a space-optimized dynamic programming approach to find the length of the longest strictly increasing subsequence.
int main() {
    vector<int> arr = {10, 9, 2, 5, 3, 7, 101, 18};

    cout << "Length of LIS : " << hyper_optimized_approach(arr) << endl;
    return 0;
}