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
int recursion_approach(int i, int k, const vector<int> &arr){    // front partitioning
    // base case
    if(i == arr.size()){
        return 0;
    }
    int maxAns = INT_MIN;
    int maxi = INT_MIN;
    int len = 0;
    for(int j = i ; j < (i + k) && j < arr.size() ; j++){
        len++;
        maxi = max(maxi, arr[j]);
        // finding max locally for a partition (ex : [1 15 7] -> max = 15 -> [15 15 15] -> sum = 15(maxi) * 3(len) = 45)
        int sum = (maxi * len) + recursion_approach(j + 1, k, arr);     // recurse for next partition (independent)
        maxAns = max(maxAns, sum);
    }
    return maxAns;
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// (BOTTOM-UP approach) (TC : O(n*k), SC : O(n))
int hyper_optimized_approach(vector<int> &arr, int k){
    int n = arr.size();
    vector<int> dp(n + 1, 0);  // dp[i] -> max sum for subarray arr[0...i-1]

    dp[n] = 0; // base case

    for(int i = n-1 ; i >= 0 ; i--){
        int maxAns = INT_MIN;
        int maxi = INT_MIN;
        int len = 0;
        for(int j = i ; j < (i + k) && j < n ; j++){
            len++;
            maxi = max(maxi, arr[j]);
            int sum = (maxi * len) + dp[j + 1]; // dp[j + 1] -> max sum for subarray arr[0...j]
            maxAns = max(maxAns, sum);
        }
        dp[i] = maxAns;
    }
    return dp[0];
}

// PROMPT: Given an integer array arr and an integer k, implement a space-optimized dynamic programming approach to partition the array into contiguous subarrays of length at most k. After partitioning, each subarray's elements are replaced with its maximum value. Return the maximum sum possible after partitioning.
int main() {
    int n = 7;                      
    int k = 3;                      
    vector<int> arr = {1, 15, 7, 9, 2, 5, 10};   
    
    cout << "Max sum : " << hyper_optimized_approach(arr, k) << endl;
    return 0;
}
