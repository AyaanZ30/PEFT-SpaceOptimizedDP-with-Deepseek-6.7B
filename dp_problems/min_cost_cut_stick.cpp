# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int j, vector<int> &cuts){
    // base case
    if(i > j){
        return 0;
    }
    int mini = 1e9;

    // recursive logic to partition the stick at different cut positions in arr
    for(int idx = i ; idx <= j ; idx++){
        int cost = (cuts[j+1] - cuts[i-1]) + recursion_approach(i, idx-1, cuts) + recursion_approach(idx+1, j, cuts);
        mini = min(mini, cost);
    }
    return mini;
}

// OPTIMIZED
// @explanation: Space-optimized 2D DP 
// (BOTTOM-UP approach) (TC : O(n^3), SC : O(n^2))
int hyper_optimized_approach(vector<int> &cuts){
    int n = cuts.size();
    vector<vector<int>> dp(n+2 ,vector<int>(n+2, 0));

    // main logic (always loop in oppposite fashion as compared to recursion as bottom up)
    for(int i = n ; i >= 1 ; i--){
        for(int j = 1 ; j <= n ; j++){
            if(i > j){continue;}
            int mini = 1e9;

            for(int idx = i ; idx<= j ; idx++){
                int cost = (cuts[j+1] - cuts[i-1]) + dp[i][idx-1] + dp[idx+1][j];
                dp[i][j] = min(mini, cost);
            }
            dp[i][j] = mini;
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
int main() {
    int l = 9;  
    vector<int> arr = {5, 6, 1, 4, 2}; 

    arr.push_back(l);
    arr.insert(arr.begin(), 0);
    sort(arr.begin(), arr.end());
    
    cout << "Min cost to cut the stick : " << hyper_optimized_approach(arr) << endl;
    return 0;
}
