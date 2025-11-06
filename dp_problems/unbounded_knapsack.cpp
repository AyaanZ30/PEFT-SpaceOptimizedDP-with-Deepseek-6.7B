#include <iostream>
#include <vector>
#include <numeric>
#include <climits>

using namespace std;

// NAIVE
// @explanation: Brute force recursion
// @complexity: Time: O(Exponential), Space: O(maxWeight) for recursion stack depth in the worst case.
int recursion_approach(int idx, int maxWeight, vector<int> &weights, vector<int> &values){
    if(idx == 0){
        return (weights[0] <= maxWeight) ? (int(maxWeight / weights[0]) * values[0]) : 0;
    }

    int notTake = recursion_approach(idx - 1, maxWeight, weights, values);
    int take = INT_MIN;
    if(weights[idx] <= maxWeight){
        take = values[idx] + recursion_approach(idx, maxWeight - weights[idx], weights, values);
    }
    return max(take, notTake);
}

// OPTIMIZED
// @explanation: Space-optimized 1D-DP using a single 1D array. The inner loop runs forwards, allowing an item to contribute to the knapsack value multiple times in the same pass.
// @complexity: Time: O(n * maxWeight), Space: O(maxWeight).
int hyper_optimized_approach(int n, int maxWeight, vector<int> &weights, vector<int> &values){
    vector<int> prev(maxWeight + 1, 0);
    for(int w = 0 ; w <= maxWeight ; w++){
        prev[w] = (int(w / weights[0]) * values[0]);
    }

    // Iterate through the rest of the items.
    for(int idx = 1 ; idx < n ; idx++){
        // Iterate forwards through the weights.
        for(int w = 0 ; w <= maxWeight ; w++){
            int notTake = 0 + prev[w];
            int take = 0;
            if(weights[idx] <= w){
                // prev[w - weights[idx]] already contains the optimized value
                // which might include the current item.
                take = values[idx] + prev[w - weights[idx]];
            }
            prev[w] = max(take, notTake);
        }
    }
    return prev[maxWeight];
}

int space_optimized_approach(int n, int maxWeight, vector<int> &weights, vector<int> &values) {
    vector<int> prev(maxWeight + 1, 0), curr(maxWeight + 1, 0);
    for(int w = 0 ; w <= maxWeight ; w++){
        prev[w] = (int(w / weights[0]) * values[0]);
    }

    // Iterate through the rest of the items.
    for(int idx = 1 ; idx < n ; idx++){
        // Iterate forwards through the weights.
        for(int w = 0 ; w <= maxWeight ; w++){
            int notTake = 0 + prev[w];
            int take = 0;
            if(weights[idx] <= w){
                take = values[idx] + curr[w - weights[idx]];
            }
            curr[w] = max(take, notTake);
        }
        prev = curr;
    }
    return prev[maxWeight];
}

// PROMPT: You are given n items with given weights and values; find the maximum total value that can be obtained in an unbounded knapsack of capacity maxWeight using a space-optimized DP approach
int main() {
    vector<int> weights = {5, 7, 9, 11};
    vector<int> values  = {10, 13, 18, 20};
    int maxWeight = 25;
    int n = weights.size();

    cout << "Max value : " << hyper_optimized_approach(n, maxWeight, weights, values) << endl;

    return 0;
}