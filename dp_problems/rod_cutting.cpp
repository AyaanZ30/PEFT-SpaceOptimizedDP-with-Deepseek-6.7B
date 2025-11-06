#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

// NAIVE
// @explanation: Brute-force recursive solution for Rod Cutting. This is a variation of Unbounded Knapsack where rod lengths are weights and prices are values.
// @complexity: Time: O(Exponential), Space: O(N) for recursion stack depth.
int recursion_approach(int idx, int N, vector<int>& prices) {
    // Base case: At the first piece (length 1).
    if (idx == 0) {
        // We can make N cuts of length 1.
        return N * prices[0];
    }

    // Don't make a cut of the current length (idx + 1).
    int not_take = recursion_approach(idx - 1, N, prices);

    // Make a cut of the current length.
    int take = INT_MIN;
    int rod_length = idx + 1;
    if (rod_length <= N) {
        // We can make more cuts of the same length.
        take = prices[idx] + recursion_approach(idx, N - rod_length, prices);
    }

    return max(take, not_take);
}

// OPTIMIZED
// @explanation: Space-optimized DP using a single 1D array. It iterates through pieces and rod lengths to find the maximum price, equivalent to Unbounded Knapsack.
// @complexity: Time: O(N * N), Space: O(N).
int hyper_optimized_approach(int n, vector<int>& prices) {
    vector<int> prev(n + 1, 0);

    // Base case: maximum price for a rod of length 'i' using only pieces of length 1.
    for (int i = 0; i <= n; i++) {
        prev[i] = i * prices[0];
    }

    // Iterate through all possible piece lengths (2 to n).
    for (int idx = 1; idx < n; idx++) {
        // Iterate through all possible rod lengths.
        for (int N = 0; N <= n; N++) {
            int notTake = prev[N];
            int take = INT_MIN;
            int rod_length = idx + 1;
            if (rod_length <= N) {
                take = prices[idx] + prev[N - rod_length];
            }
            prev[N] = max(take, notTake);
        }
    }
    return prev[n];
}

// PROMPT: You are given a rod of length n and an array where each element represents the price of a rod piece of that length (from 1 to n). Formulate a space-optimized dynamic programming solution to determine the maximum profit obtainable by cutting the rod into smaller pieces and selling them. You can make unlimited cuts of any available length.
int main() {
    vector<int> prices = {1, 5, 8, 9, 10, 17, 17, 20}; // prices for lengths 1 to 8
    int n = prices.size();

    cout << "Max profit : " << hyper_optimized_approach(n, prices) << endl;
    return 0;
}

