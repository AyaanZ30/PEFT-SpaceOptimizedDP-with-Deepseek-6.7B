#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// NAIVE
int recursion_approach(int idx, int target, vector<int>& arr) {
    // Base case: If we are at the first element.
    if (idx == 0) {
        if(target == 0 && arr[0] == 0) return 2; // Two choices: pick or not pick the 0
        if(target == 0 || target == arr[0]) return 1; // One choice
        return 0;
    }

    // Don't include the current element in the subset.
    int not_take = recursion_approach(idx - 1, target, arr);

    // Include the current element if its value is not greater than the remaining target.
    int take = 0;
    if (arr[idx] <= target) {
        take = recursion_approach(idx - 1, target - arr[idx], arr);
    }

    // Return the sum of possibilities from both branches.
    return take + not_take;
}


// OPTIMIZED
// @explanation: Space-optimized DP using a single 1D integer array (prev) to store the counts of subsets for each possible sum.
// @complexity: Time: O(n * target), Space: O(target).
int hyper_optimized_approach(int n, int k, vector<int>& arr) {
    // prev[i] will store the count of subsets with sum 'i'.
    vector<int> prev(k + 1, 0);

    // Base case for the first element
    if(arr[0] == 0) prev[0] = 2; // Two choices for 0: pick or not pick
    else prev[0] = 1; // Only one choice for non-zero: not pick
    
    if(arr[0] != 0 && arr[0] <= k) prev[arr[0]] = 1;


    // Iterate through the rest of the array elements.
    for (int idx = 1; idx < n; idx++) {
        // Iterate backwards from the target sum.
        for (int target = k; target >= 0; target--) {
            int not_take = prev[target];
            int take = 0;
            if(arr[idx] <= target) take = prev[target - arr[idx]];
            
            prev[target] = take + not_take;
        }
    }
    
    // The final answer is the number of ways to form the target sum 'k'.
    return prev[k];
}

// PROMPT: You are given an array of integers and a target sum. Construct a space-optimized dynamic programming solution that counts the total number of subsets within the array that sum exactly to the given target value.
int main() {
    vector<int> arr = {1, 2, 3, 3};
    int target = 6;
    int n = arr.size();

    cout << "Number of subsets" << hyper_optimized_approach(n, target, arr) << endl;
    return 0;
}

