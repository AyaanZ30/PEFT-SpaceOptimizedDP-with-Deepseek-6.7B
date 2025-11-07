#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

using namespace std;

// NAIVE
// @explanation: Brute-force recursion 
// @complexity: Time: O(2^n), Space: O(n) for recursion stack.
void recursion_approach(int idx, int current_sum, int total_sum, vector<int> &arr, int &min_diff) {
    // Base case: when all elements have been considered.
    if (idx < 0) {
        // The two subset sums are current_sum and (total_sum - current_sum).
        min_diff = min(min_diff, abs((total_sum - current_sum) - current_sum));
        return;
    }

    // Case 1: Include arr[idx] in the current subset.
    recursion_approach(idx - 1, current_sum + arr[idx], total_sum, arr, min_diff);

    // Case 2: Do not include arr[idx] in the current subset.
    recursion_approach(idx - 1, current_sum, total_sum, arr, min_diff);
}


// OPTIMIZED
// @explanation: Space-optimized 1D-DP. 
// @complexity: Time: O(n * sum), Space: O(sum).
int hyper_optimized_approach(vector<int>& arr) {
    int total_sum = 0;
    int n = arr.size();
    for (int x : arr) {
        total_sum += x;
    }

    int k = total_sum;
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

    int min_diff = 1e9;
    // The possible sums for one subset are marked as 'true' in the prev array.
    // We only need to check up to total_sum/2.
    for (int s1 = 0; s1 <= total_sum / 2; s1++) {
        if (prev[s1]) {
            // s2 = total_sum - s1
            // diff = abs(s2 - s1) = abs(total_sum - 2*s1)
            min_diff = min(min_diff, abs(total_sum - 2 * s1));
        }
    }
    return min_diff;
}

// PROMPT: You are given an array of positive integers. Write a space-optimized dynamic programming solution to find the minimum possible absolute difference between the sums of two subsets formed from this array.
int main() {
    vector<int> arr = {1, 6, 11, 5};    
    cout << "Minimum subset sum difference (Optimized): " << hyper_optimized_approach(arr) << endl;

    return 0;
}
