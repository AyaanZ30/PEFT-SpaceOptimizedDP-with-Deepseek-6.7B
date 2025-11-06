# include <iostream>
# include <string>
# include <vector>
# include <algorithm>
# include <climits>

using namespace std;


// NAIVE (TC : O(exponential) SC : O(n) auxiliary stack space)
int recursion_approach(int i, int j, const vector<int> &arr){
    if(i == j){
        return 0;
    }
    int mini = 1e9;

    // recursive logic to partition the matrix at different k values (ABCD -> (A)(BCD) , (AB)(CD) , (ABC)(D))
    for(int k = i ; k < j ; k++){
        int steps = (arr[i-1] * arr[k] * arr[j]) + recursion_approach(i, k, arr) + recursion_approach(k+1, j, arr);
        mini = min(mini, steps);
    }
    return mini;
}

// OPTIMIZED
// @explanation: Space-optimized 2D DP 
// (BOTTOM-UP approach) (TC : O(n^3), SC : O(n^2))
int hyper_optimized_approach(const vector<int> &arr, int size){
    int n = arr.size();
    vector<vector<int>> dp(n ,vector<int>(n, 0));

    // main logic
    for(int i = 0 ; i < n ; i++){
        dp[i][i] = 0;                   // analogous to rec base cases
    }
    for(int i = n-1 ; i >= 1 ; i--){
        for(int j = i+1 ; j < n ; j++){
            int mini = 1e9;
            for(int k = i ; k <= j-1 ; k++){
                int steps = (arr[i-1] * arr[k] * arr[j]) + dp[i][k] + dp[k+1][j];
                mini = min(mini, steps);
            }
            dp[i][j] = mini;
        }
    }
    return dp[1][n-1];
}

void printArray(const vector<int> &arr){
    cout << "Entered Array: ";
    for(auto x: arr){
        cout << x << " ";
    }
    cout << endl;
}
int main() {
    vector<int> arr = {10, 30, 5, 60};  
    int n = arr.size();

    cout << "Min number of operations : " << hyper_optimized_approach(arr, n) << endl;
    return 0;
}




