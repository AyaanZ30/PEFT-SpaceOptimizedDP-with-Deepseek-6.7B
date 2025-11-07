#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// NAIVE
// @explanation: Brute-force recursion that explores both adding and subtracting each number to find how many combinations result in the target sum.
// @complexity: Time: O(2^n), Space: O(n) for recursion stack.
int recursion_approach(int idx, int current_sum, int target, vector<int>& nums) {
    // Base case: When all numbers have been processed.
    if (idx < 0) {
        return (current_sum == target) ? 1 : 0;
    }

    // Explore the path where we subtract the current number.
    int subtract_count = recursion_approach(idx - 1, current_sum - nums[idx], target, nums);

    // Explore the path where we add the current number.
    int add_count = recursion_approach(idx - 1, current_sum + nums[idx], target, nums);

    // Total ways is the sum of ways from both paths.
    return subtract_count + add_count;
}

// OPTIMIZED
// @explanation: This problem is transformed into 'Count Subsets with Given Sum'. We find the number of subsets that sum to (total_sum + target) / 2. This is solved with a 1D DP array.
// @complexity: Time: O(n * sum), Space: O(sum).
int hyper_optimized_approach(vector<int>& nums, int target) {
    int n = nums.size();
    int total_sum = 0;
    for(int x : nums) total_sum += x;

    // Conditions for impossibility.
    // 1. The target sum must be achievable.
    // 2. (total_sum - target) must be non-negative and even.
    if (total_sum - target < 0 || (total_sum - target) % 2 != 0) {
        return 0;
    }

    // This problem reduces to finding the number of subsets with sum s2,
    // where s2 = (total_sum - target) / 2.
    int s2 = (total_sum - target) / 2;
    
    vector<int> prev(s2 + 1, 0);
    prev[0] = 1;
    
    // This is now the 'Count Subsets with Sum' problem.
    for (int i = 0; i < n; i++) {
        for (int t = s2; t >= nums[i]; t--) {
            prev[t] = prev[t] + prev[t - nums[i]];
        }
    }
    return prev[s2];
}

// PROMPT: You are given an array of integers and a target value. Each element can be assigned either a '+' or '-' sign. Write a space-optimized dynamic programming solution to count the total number of ways to assign these signs so that the resulting expression evaluates exactly to the target sum.
int main() {
    vector<int> nums = {1, 1, 1, 1, 1};
    int target = 3;
    cout << "Number of ways to get target sum : "  << hyper_optimized_approach(nums, target) << endl;

    return 0;
}

