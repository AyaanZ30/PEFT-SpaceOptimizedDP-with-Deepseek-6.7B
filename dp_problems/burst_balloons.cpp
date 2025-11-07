# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int j, const vector<int> &cost){
    if(i > j){
        return 0;
    }
    int maxi = -1e9;
    // core logic (starting from bursting a single balloon and working upwards by recursively solving independent subproblems)
    for(int idx = i ; idx <= j ; idx++){
        int coins = (cost[i-1] * cost[idx] * cost[j+1]) + 
        recursion_approach(i, idx-1, cost) + recursion_approach(idx+1, j, cost);
        maxi = max(maxi, coins);
    }
    return maxi;
}

// OPTIMIZED
// @explanation: Space-optimized 2D DP 
// (BOTTOM-UP approach) (TC : O(n^3) SC : O(n^2) for DP table)
int hyper_optimized_approach(vector<int> &costs){
    int n = costs.size();
    vector<vector<int>> dp(n+2 ,vector<int>(n+2, 0));

    // main logic (always loop in oppposite fashion as compared to recursion as bottom up)
    for(int i = n ; i >= 1 ; i--){
        for(int j = 1 ; j <= n ; j++){
            if(i > j){continue;}
            int maxi = -1e9;

            for(int idx = i ; idx <= j ; idx++){
                int cost = (costs[i-1] * costs[idx] * costs[j+1]) + dp[i][idx-1] + dp[idx+1][j];
                maxi = max(maxi, cost);
            }
            dp[i][j] = maxi;
        }
    }
    return dp[1][n];
}

void printArray(const vector<int> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}

// PROMPT: Given an array of balloons with numbers written on them, implement a space-optimized dynamic programming approach to find the maximum coins obtainable by bursting all balloons. When a balloon i is burst, you get nums[i-1] * nums[i] * nums[i+1] coins, and the balloons i-1 and i+1 become adjacent.
int main(){
    int n = 4;
    vector<int> arr = {3,1,5,8};

    arr.push_back(1);
    arr.insert(arr.begin(), 1);

    cout << "Max coins obtained : " << hyper_optimized_approach(arr) << endl;
    return 0;
}
