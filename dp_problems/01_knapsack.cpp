# include <iostream>
# include <algorithm>
# include <vector>
#include <climits>

using namespace std;

// NAIVE 
int recursion_approach(int idx, int maxWeight, vector<int> &weights, vector<int> &values){
    if(idx == 0){
        return ((weights[idx] <= maxWeight) ? values[idx] : 0);
    }
    int not_take = 0 + recursion_approach(idx-1, maxWeight, weights, values);
    int take = INT_MIN;
    if(weights[idx] <= maxWeight){
        take = values[idx] + recursion_approach(idx-1, maxWeight - weights[idx], weights, values);
    }
    cout << "Time Complexity: O(2^n)" << endl;
    cout << "Space Complexity: O(n)  (recursion stack space)" << endl;
    return max(take, not_take);
}

// OPTIMIZED
// @explanation: Space-optimized 1D DP 
// @complexity: Time: O(n * maxWeight), Space: O(maxWeight).
int hyper_optimized_approach(vector<int> &weights, vector<int> &values, int n, int maxWeight){
    vector<int> prev(maxWeight + 1, 0);
    for(int w = weights[0] ; w <= maxWeight ; w++){
        prev[w] = values[0];
    }
    for(int idx = 1 ; idx < n ; idx++){
        for(int w = maxWeight ; w >= 0 ; w--){
            int notTake = 0 + prev[w];
            int take = INT_MIN;
            if(weights[idx] <= w){
                take = values[idx] + prev[w - weights[idx]];
            }
            prev[w] = max(take, notTake);
        }
    }
    return prev[maxWeight];
}

int space_optimized_approach(vector<int> &weights, vector<int> &values, int n, int maxWeight){
    vector<int> prev(maxWeight + 1, 0);
    vector<int> curr(maxWeight + 1, 0);
    for(int w = weights[0] ; w <= maxWeight ; w++){
        prev[w] = values[0];
    }
    for(int idx = 1 ; idx < n ; idx++){
        for(int w = 0 ; w <= maxWeight ; w++){
            int notTake = 0 + prev[w];
            int take = INT_MIN;
            if(weights[idx] <= w){
                take = values[idx] + prev[w - weights[idx]];
            }
            curr[w] = max(take, notTake);
        }
        prev = curr;
    }
    cout << "Time Complexity: O(n * maxWeight)" << endl;
    cout << "Space Complexity: O(maxWeight) (1-D DP used)" << endl;
    return prev[maxWeight];
}

int tabulation_approach(vector<int> &weights, vector<int> &values, int n, int maxWeight){
    vector<vector<int>> dp(n, vector<int>(maxWeight + 1, 0));
    for(int w = weights[0] ; w <= maxWeight ; w++){
        dp[0][w] = values[0];
    }
    for(int idx = 1 ; idx < n ; idx++){
        for(int w = 0 ; w <= maxWeight ; w++){
            int notTake = 0 + dp[idx - 1][w];
            int take = INT_MIN;
            if(weights[idx] <= w){
                take = values[idx] + dp[idx - 1][w - weights[idx]];
            }
            dp[idx][w] = max(take, notTake);
        }
    }
    cout << "Time Complexity: O(n * maxWeight)" << endl;
    cout << "Space Complexity: O(n * maxWeight)" << endl;
    return dp[n - 1][maxWeight];     
}

int knapsack(vector<int> &weights, vector<int> &values, int n, int maxWeight){
    // return recursion_approach(n-1, maxWeight, weights, values);
    return hyper_optimized_approach(weights, values, n, maxWeight);
}

// PROMPT: You are given two arrays: one representing the weights of items and the other their corresponding values. You are also given the capacity of a knapsack. Formulate a space-optimized dynamic programming algorithm to find the maximum total value that can be obtained without exceeding the knapsack’s capacity.
int main(){
    int n = 3;
    int bag_capacity = 50;
    vector<int> weights = {10, 20, 30};
    vector<int> values = {60, 100, 120};

    cout << "Max cost in knapsack : " << hyper_optimized_approach(weights, values, n, bag_capacity) << endl;
    return 0;
}
