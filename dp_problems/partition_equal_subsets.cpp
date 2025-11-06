#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

// NAIVE
// @explanation: Brute-force recursion 
// @complexity: Time: O(2^n), Space: O(n) for recursion stack.
bool recursion_approach(int idx, int target, vector<int> &arr) {
    if (target == 0) return true;
    if (idx == 0) return (arr[0] == target);

    bool not_take = recursion_approach(idx - 1, target, arr);
    bool take = false;
    if (arr[idx] <= target) {
        take = recursion_approach(idx - 1, target - arr[idx], arr);
    }
    return take || not_take;
}
    
// OPTIMIZED
// @explanation: Space-optimized 1D DP (sum check for k = total_sum/2)
// @complexity: Time: O(n * sum), Space: O(sum).
bool hyper_optimized_approach(int n, vector<int> &arr) {
    int total_sum = 0;
    for (int x : arr) {
        total_sum += x;
    }
    // If total sum is odd, it cannot be partitioned into two equal halves.
    if (total_sum % 2 != 0) {
        return false;
    }
    int k = total_sum / 2;
    vector<bool> prev(k + 1, false);
    prev[0] = true;
    if (arr[0] <= k) {
        prev[arr[0]] = true;
    }

    for (int idx = 1; idx < n; idx++) {
        for (int target = k; target >= arr[idx]; target--) {
            prev[target] = prev[target] || prev[target - arr[idx]];
        }
    }
    return prev[k];
}

bool canPartition(vector<int>& arr) {
    int n = arr.size();
    int total_sum = 0;
    for(int x : arr) total_sum += x;
    if(total_sum % 2) return false;

    // return recursion_approach(n - 1, total_sum / 2, arr);
    return hyper_optimized_approach(n, arr);
}

// PROMPT: You are given an array of positive integers. Implement a space-optimized dynamic programming approach to determine whether the array can be partitioned into two subsets such that the sum of elements in both subsets is equal.
int main() {
    vector<int> arr = {10, 20, 15, 5, 25, 30};
    int n = arr.size();
    
    cout << hyper_optimized_approach(n, arr);

    return 0;
}
