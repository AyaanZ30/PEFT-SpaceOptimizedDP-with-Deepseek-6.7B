#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// NAIVE
int recursion_approach(int idx, int amount, vector<int>& coins) {
    // Base case: At the first coin.
    if (idx == 0) {
        // If the amount is divisible by the coin value.
        if (amount % coins[0] == 0) {
            return amount / coins[0];
        }
        // Otherwise, it's not possible to make the amount.
        return 1e9; // A large value indicating impossibility.
    }

    // Don't take the current coin.
    int not_take = recursion_approach(idx - 1, amount, coins);

    // Take the current coin.
    int take = 1e9;
    if (coins[idx] <= amount) {
        // 1 (for the current coin) + result for the remaining amount.
        take = 1 + recursion_approach(idx, amount - coins[idx], coins);
    }

    return min(take, not_take);
}

// OPTIMIZED
// @explanation: Space-optimized DP using a single 1D array. prev[i] stores the minimum coins needed for amount 'i'.
// @complexity: Time: O(n * amount), Space: O(amount).
int hyper_optimized_approach(int n, int amount, vector<int>& coins) {
    vector<int> prev(amount + 1, 1e9);

    // Base case: 0 coins are needed for amount 0.
    prev[0] = 0;
    
    // Initialize for the first coin.
    for(int T=0; T<=amount; T++){
        if(T % coins[0] == 0)
            prev[T] = T / coins[0];
    }

    // Iterate through the rest of the coins.
    for (int idx = 1; idx < n; idx++) {
        // Iterate through all amounts.
        for (int T = 0; T <= amount; T++) {
            int notTake = prev[T];
            int take = 1e9;
            if (coins[idx] <= T) {
                take = 1 + prev[T - coins[idx]];
            }
            prev[T] = min(take, notTake);
        }
    }
    
    int result = prev[amount];
    return (result >= 1e9) ? -1 : result; // Return -1 if not possible.
}

// PROMPT: You are given an array of coin denominations and a target amount. Develop a space-optimized dynamic programming algorithm to determine the minimum number of coins required to make up that amount. If it is not possible to form the amount, return -1.
int main() {
    vector<int> coins = {1, 2, 5};
    int amount = 11;
    int n = coins.size();

    cout << "Minimum coins needed : " 
         << hyper_optimized_approach(n, amount, coins) << endl;

    return 0;
}

